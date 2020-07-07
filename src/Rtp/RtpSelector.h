﻿/*
 * Copyright (c) 2016 The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/xiongziliang/ZLMediaKit).
 *
 * Use of this source code is governed by MIT license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#ifndef ZLMEDIAKIT_RTPSELECTOR_H
#define ZLMEDIAKIT_RTPSELECTOR_H

#if defined(ENABLE_RTPPROXY)
#include <stdint.h>
#include <mutex>
#include <unordered_map>
#include "RtpProcess.h"
#include "Common/MediaSource.h"

namespace mediakit{

class RtpSelector;
class RtpProcessHelper : public MediaSourceEvent , public std::enable_shared_from_this<RtpProcessHelper> {
public:
    typedef std::shared_ptr<RtpProcessHelper> Ptr;
    RtpProcessHelper(const string &stream_id, const weak_ptr<RtpSelector > &parent);
    ~RtpProcessHelper();
    void attachEvent();
    RtpProcess::Ptr & getProcess();

protected:
    // 通知其停止推流
    bool close(MediaSource &sender,bool force) override;
    // 观看总人数
    int totalReaderCount(MediaSource &sender) override;

private:
    weak_ptr<RtpSelector > _parent;
    RtpProcess::Ptr _process;
    string _stream_id;
};

class RtpSelector : public std::enable_shared_from_this<RtpSelector>{
public:
    RtpSelector();
    ~RtpSelector();

    static bool getSSRC(const char *data,int data_len, uint32_t &ssrc);
    static RtpSelector &Instance();

    bool inputRtp(const Socket::Ptr &sock, string &stream_id, const char *data, int data_len,
                  const struct sockaddr *addr, uint32_t *dts_out = nullptr);

    RtpProcess::Ptr getProcess(const string &stream_id, bool makeNew);
    void delProcess(const string &stream_id, const RtpProcess *ptr);

private:
    void onManager();
    void createTimer();

private:
    unordered_map<string,RtpProcessHelper::Ptr> _map_rtp_process;
    recursive_mutex _mtx_map;
    Timer::Ptr _timer;
};

}//namespace mediakit
#endif//defined(ENABLE_RTPPROXY)
#endif //ZLMEDIAKIT_RTPSELECTOR_H
