/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef __AAL_H_
#define __AAL_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	AAL_SUCCESS = 0,
	AAL_PAUSED,
	AAL_ERROR,
	AAL_UNKNOWN
} aal_status_t;

typedef enum {
	AAL_STREAM_LPCM = 0,
	AAL_STREAM_MP3
} aal_stream_type_t;

typedef struct {
	void (*on_start) (void *user_data);
	void (*on_stop) (aal_status_t reason, void *user_data);
	void (*on_almost_done) (void *user_data);
	void (*on_data) (const int16_t *data, const size_t length, void *user_data);
	void (*on_data_requested) (void *user_data);
} aal_listener_t;

typedef struct {
	const char *name;
	const char *device;
	int rate;
	const char *uri;
	const aal_listener_t *listener;
	void *user_data;
	int module_id;
} aal_attributes_t;

typedef void* aal_handle_t;

int aal_get_module_count();
const char *aal_get_module_name(int module_id);
bool aal_initialize(int module_id);
void aal_deinitialize(int module_id);

typedef void aal_log_func(int level, const char *data, int len);
void aal_set_log_func(aal_log_func* func);

aal_handle_t aal_player_create(const aal_attributes_t *attr);
void aal_player_play(aal_handle_t handle);
void aal_player_pause(aal_handle_t handle);
void aal_player_stop(aal_handle_t handle);
int64_t aal_player_get_position(aal_handle_t handle);
int64_t aal_player_get_duration(aal_handle_t handle);
void aal_player_seek(aal_handle_t handle, int64_t position);
void aal_player_set_volume(aal_handle_t handle, double volume);
void aal_player_set_mute(aal_handle_t handle, bool mute);
ssize_t aal_player_write(aal_handle_t handle, const char *data, const size_t size);
void aal_player_notify_end_of_stream(aal_handle_t handle);
void aal_player_set_stream_type(aal_handle_t handle, const aal_stream_type_t type);
void aal_player_destroy(aal_handle_t handle);

aal_handle_t aal_recorder_create(const aal_attributes_t *attr);
void aal_recorder_play(aal_handle_t handle);
void aal_recorder_stop(aal_handle_t handle);
void aal_recorder_destroy(aal_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif // __AAL_H_