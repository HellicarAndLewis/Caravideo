/*
    $Id: dump_yaml.c 231 2011-06-27 13:46:19Z marc.noirot $

    FLV Metadata updater

    Copyright (C) 2007-2012 Marc Noirot <marc.noirot AT gmail.com>

    This file is part of FLVMeta.

    FLVMeta is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLVMeta is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLVMeta; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "dump.h"
#include "dump_yaml.h"
#include "yaml.h"

#include <stdio.h>
#include <string.h>

/* YAML metadata dumping */
static void amf_data_yaml_dump(const amf_data * data, yaml_emitter_t * emitter) {
    if (data != NULL) {
        amf_node * node;
        yaml_event_t event;
        time_t time;
        struct tm * t;
        char str[128];

        switch (data->type) {
            case AMF_TYPE_NUMBER:
                sprintf(str, "%.12g", data->number_data);
                yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_BOOLEAN:
                sprintf(str, (data->boolean_data) ? "true" : "false");
                yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_STRING:
                yaml_scalar_event_initialize(&event, NULL, NULL, amf_string_get_bytes(data), (int)amf_string_get_size(data), 1, 1, YAML_ANY_SCALAR_STYLE);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_OBJECT:
                yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
                yaml_emitter_emit(emitter, &event);
                node = amf_object_first(data);
                while (node != NULL) {
                    amf_data * name;
                    name = amf_object_get_name(node);
                    /* if this fails, we skip the current entry */
                    if (yaml_scalar_event_initialize(&event, NULL, NULL, amf_string_get_bytes(name), amf_string_get_size(name), 1, 1, YAML_ANY_SCALAR_STYLE)) {
                        yaml_emitter_emit(emitter, &event);
                        amf_data_yaml_dump(amf_object_get_data(node), emitter);
                    }
                    node = amf_object_next(node);
                }
                yaml_mapping_end_event_initialize(&event);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_NULL:
            case AMF_TYPE_UNDEFINED:
                yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"null", 4, 1, 1, YAML_ANY_SCALAR_STYLE);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_ASSOCIATIVE_ARRAY:
                yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
                yaml_emitter_emit(emitter, &event);
                node = amf_associative_array_first(data);
                while (node != NULL) {
                    amf_data * name;
                    name = amf_associative_array_get_name(node);
                    /* if this fails, we skip the current entry */
                    if (yaml_scalar_event_initialize(&event, NULL, NULL, amf_string_get_bytes(name), amf_string_get_size(name), 1, 1, YAML_ANY_SCALAR_STYLE)) {
                        yaml_emitter_emit(emitter, &event);
                        amf_data_yaml_dump(amf_associative_array_get_data(node), emitter);
                    }
                    node = amf_associative_array_next(node);
                }
                yaml_mapping_end_event_initialize(&event);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_ARRAY:
                yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_SEQUENCE_STYLE);
                yaml_emitter_emit(emitter, &event);
                node = amf_array_first(data);
                while (node != NULL) {
                    amf_data_yaml_dump(amf_array_get(node), emitter);
                    node = amf_array_next(node);
                }
                yaml_sequence_end_event_initialize(&event);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_DATE:
                time = amf_date_to_time_t(data);
                tzset();
                t = localtime(&time);
                strftime(str, sizeof(str), "%Y-%m-%dT%H:%M:%S", t);
                yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
                yaml_emitter_emit(emitter, &event);
                break;
            case AMF_TYPE_XML: break;
            case AMF_TYPE_CLASS: break;
            default: break;
        }
    }
}

/* YAML FLV file full dump callbacks */

static int yaml_on_header(flv_header * header, flv_parser * parser) {
    yaml_emitter_t * emitter;
    yaml_event_t event;
    char buffer[20];

    emitter = (yaml_emitter_t *)parser->user_data;

    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* magic */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"magic", 5, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%.3s", header->signature);
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* hasVideo */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"hasVideo", 8, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%s", flv_header_has_video(*header) ? "true" : "false");
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* hasAudio */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"hasAudio", 8, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%s", flv_header_has_audio(*header) ? "true" : "false");
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* version */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"version", 7, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%i", header->version);
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* start of tags array */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"tags", 4, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_SEQUENCE_STYLE);
    yaml_emitter_emit(emitter, &event);

    return OK;
}

static int yaml_on_tag(flv_tag * tag, flv_parser * parser) {
    const char * str;
    yaml_emitter_t * emitter;
    yaml_event_t event;
    char buffer[20];

    emitter = (yaml_emitter_t *)parser->user_data;

    str = dump_string_get_tag_type(tag);

    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* type */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"type", 4, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* timestamp */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"timestamp", 9, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%i", flv_tag_get_timestamp(*tag));
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* data size */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"dataSize", 8, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%i", flv_tag_get_body_length(*tag));
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    /* offset */
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"offset", 6, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    sprintf(buffer, "%" FILE_OFFSET_PRINTF_FORMAT "i", parser->stream->current_tag_offset);
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)buffer, (int)strlen(buffer), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    return OK;
}

static int yaml_on_video_tag(flv_tag * tag, flv_video_tag vt, flv_parser * parser) {
    const char * str;
    yaml_emitter_t * emitter;
    yaml_event_t event;

    emitter = (yaml_emitter_t *)parser->user_data;

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"videoData", 9, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
    yaml_emitter_emit(emitter, &event);

    str = dump_string_get_video_codec(vt);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"codecID", 7, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    str = dump_string_get_video_frame_type(vt);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"frameType", 9, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(emitter, &event);

    return OK;
}

static int yaml_on_audio_tag(flv_tag * tag, flv_audio_tag at, flv_parser * parser) {
    const char * str;
    yaml_emitter_t * emitter;
    yaml_event_t event;

    emitter = (yaml_emitter_t *)parser->user_data;

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"audioData", 9, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
    yaml_emitter_emit(emitter, &event);

    str = dump_string_get_sound_type(at);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"type", 4, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    str = dump_string_get_sound_size(at);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"size", 4, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    str = dump_string_get_sound_rate(at);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"rate", 4, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    str = dump_string_get_sound_format(at);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"format", 6, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str, (int)strlen(str), 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(emitter, &event);

    return OK;
}

static int yaml_on_metadata_tag(flv_tag * tag, amf_data * name, amf_data * data, flv_parser * parser) {
    yaml_emitter_t * emitter;
    yaml_event_t event;

    emitter = (yaml_emitter_t *)parser->user_data;

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"scriptDataObject", 16, 1, 1, YAML_ANY_SCALAR_STYLE);
    yaml_emitter_emit(emitter, &event);

    amf_data_yaml_dump(data, emitter);

    return OK;
}

static int yaml_on_prev_tag_size(uint32 size, flv_parser * parser) {
    yaml_emitter_t * emitter;
    yaml_event_t event;

    emitter = (yaml_emitter_t *)parser->user_data;

    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(emitter, &event);

    return OK;
}

static int yaml_on_stream_end(flv_parser * parser) {
    yaml_emitter_t * emitter;
    yaml_event_t event;

    emitter = (yaml_emitter_t *)parser->user_data;

    yaml_sequence_end_event_initialize(&event);
    yaml_emitter_emit(emitter, &event);

    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(emitter, &event);

    return OK;
}

/* YAML FLV file metadata dump callbacks */
static int yaml_on_metadata_tag_only(flv_tag * tag, amf_data * name, amf_data * data, flv_parser * parser) {
    flvmeta_opts * options = (flvmeta_opts*) parser->user_data;

    if (options->metadata_event == NULL) {
        if (!strcmp((char*)amf_string_get_bytes(name), "onMetaData")) {
            dump_yaml_amf_data(data);
            return FLVMETA_DUMP_STOP_OK;
        }
    }
    else {
        if (!strcmp((char*)amf_string_get_bytes(name), options->metadata_event)) {
            dump_yaml_amf_data(data);
        }
    }
    return OK;
}

/* dumping functions */
void dump_yaml_setup_metadata_dump(flv_parser * parser) {
    if (parser != NULL) {
        parser->on_metadata_tag = yaml_on_metadata_tag_only;
    }
}

int dump_yaml_file(flv_parser * parser, const flvmeta_opts * options) {
    yaml_emitter_t emitter;
    yaml_event_t event;
    int ret;
    
    parser->on_header = yaml_on_header;
    parser->on_tag = yaml_on_tag;
    parser->on_audio_tag = yaml_on_audio_tag;
    parser->on_video_tag = yaml_on_video_tag;
    parser->on_metadata_tag = yaml_on_metadata_tag;
    parser->on_prev_tag_size = yaml_on_prev_tag_size;
    parser->on_stream_end = yaml_on_stream_end;

    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_file(&emitter, stdout);
    yaml_emitter_open(&emitter);

    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
    yaml_emitter_emit(&emitter, &event);
    
    parser->user_data = &emitter;

    ret = flv_parse(options->input_file, parser);

    yaml_document_end_event_initialize(&event, 1);
    yaml_emitter_emit(&emitter, &event);

    yaml_emitter_flush(&emitter);
    yaml_emitter_close(&emitter);
    yaml_emitter_delete(&emitter);

    return ret;
}

int dump_yaml_amf_data(const amf_data * data) {
    yaml_emitter_t emitter;
    yaml_event_t event;

    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_file(&emitter, stdout);
    yaml_emitter_open(&emitter);

    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
    yaml_emitter_emit(&emitter, &event);

    /* dump AMF into YAML */
    amf_data_yaml_dump(data, &emitter);

    yaml_document_end_event_initialize(&event, 1);
    yaml_emitter_emit(&emitter, &event);

    yaml_emitter_flush(&emitter);
    yaml_emitter_close(&emitter);
    yaml_emitter_delete(&emitter);

    return OK;
}
