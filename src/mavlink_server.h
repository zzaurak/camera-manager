/*
 * This file is part of the Dronecode Camera Manager
 *
 * Copyright (C) 2017  Intel Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <map>
#include <mavlink.h>
#include <memory>
#include <vector>

#include "CameraComponent.h"
#include "conf_file.h"
#include "socket.h"

typedef struct image_callback {
    int comp_id;             /* Component ID */
    struct sockaddr_in addr; /* Requester address */
} image_callback_t;

class MavlinkServer {
public:
    MavlinkServer(const ConfFile &conf);
    ~MavlinkServer();
    void start();
    void stop();
    int addCameraComponent(CameraComponent *camComp);
    void removeCameraComponent(CameraComponent *camComp);
    CameraComponent *getCameraComponent(int compID);

private:
    bool _is_running;
    unsigned int _timeout_handler;
    UDPSocket _udp;
    struct sockaddr_in _broadcast_addr = {};
    bool _is_sys_id_found;
    int _system_id;
    int _comp_id;
    std::map<int, CameraComponent *> compIdToObj;

    void _message_received(const struct sockaddr_in &sockaddr, const struct buffer &buf);
    void _handle_mavlink_message(const struct sockaddr_in &addr, mavlink_message_t *msg);
    void _handle_request_camera_information(const struct sockaddr_in &addr,
                                            mavlink_command_long_t &cmd);
    void _handle_request_camera_settings(const struct sockaddr_in &addr,
                                         mavlink_command_long_t &cmd);
    void _handle_request_storage_information(const struct sockaddr_in &addr,
                                             mavlink_command_long_t &cmd);
    void _handle_set_camera_mode(const struct sockaddr_in &addr,
                                                        mavlink_command_long_t &cmd);
    void _handle_image_start_capture(const struct sockaddr_in &addr, mavlink_command_long_t &cmd);
    void _handle_image_stop_capture(const struct sockaddr_in &addr, mavlink_command_long_t &cmd);
    void _handle_video_start_capture(const struct sockaddr_in &addr, mavlink_command_long_t &cmd);
    void _handle_video_stop_capture(const struct sockaddr_in &addr, mavlink_command_long_t &cmd);
    void _handle_request_video_stream_information(const struct sockaddr_in &addr, mavlink_command_long_t &cmd);
    void _image_captured_cb(image_callback_t cb_data, int result, int seq_num);
    void _handle_request_camera_capture_status(const struct sockaddr_in &addr,
                                               mavlink_command_long_t &cmd);
    void _handle_param_ext_request_read(const struct sockaddr_in &addr, mavlink_message_t *msg);
    void _handle_param_ext_request_list(const struct sockaddr_in &addr, mavlink_message_t *msg);
    void _handle_param_ext_set(const struct sockaddr_in &addr, mavlink_message_t *msg);
    void _handle_reset_camera_settings(const struct sockaddr_in &addr, mavlink_command_long_t &cmd);
    void _handle_heartbeat(const struct sockaddr_in &addr, mavlink_message_t *msg);
    bool _send_camera_capture_status(int compid, const struct sockaddr_in &addr);
    bool _send_mavlink_message(const struct sockaddr_in *addr, mavlink_message_t &msg);
    void _send_ack(const struct sockaddr_in &addr, int cmd, int comp_id, bool success);
#if 0
    const Stream::FrameSize *_find_best_frame_size(Stream &s, uint32_t w, uint32_t v);
#endif
    friend bool _heartbeat_cb(void *data);

    CameraParameters::Mode mav2dcmCameraMode(uint32_t mode);
    uint32_t dcm2mavCameraMode(CameraParameters::Mode mode);
};
