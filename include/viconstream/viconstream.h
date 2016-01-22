/****************************************************************************
*
* Copyright (C) 2015-2016 Emil Fresk.
* All rights reserved.
*
* This file is part of the cppViconStream library.
*
* GNU Lesser General Public License Usage
* This file may be used under the terms of the GNU Lesser
* General Public License version 3.0 as published by the Free Software
* Foundation and appearing in the file LICENSE included in the
* packaging of this file.  Please review the following information to
* ensure the GNU Lesser General Public License version 3.0 requirements
* will be met: http://www.gnu.org/licenses/lgpl-3.0.html.
*
* If you have questions regarding the use of this file, please contact
* Emil Fresk at emil.fresk@gmail.com.
*
****************************************************************************/

/* Data includes. */
#include <string>
#include <map>

/* Threading includes. */
#include <thread>
#include <mutex>
#include <condition_variable>

/* Vicon include. */
#include "Client.h"

#ifndef _VICONSTREAM_H
#define _VICONSTREAM_H

using namespace ViconDataStreamSDK::CPP;

namespace ViconStream
{
    typedef std::function<void(const Client &)> viconstream_callback;

    class ViconStream;
}

class ViconStream::ViconStream
{

private:
    /** @brief Mutex for the ID counter and the callback list. */
    std::mutex _id_cblock;

    /** @brief ID counter for the removal of subscriptions. */
    unsigned int _id;

    /** @brief Vector holding the registered callbacks. */
    std::map<unsigned int, viconstream_callback> callbacks;

    /** @brief Vicon client object. */
    Client _vicon_client;

    /** @brief The vicon server's address. */
    std::string _host_name;

    /** @brief Time since the ViconStream object was created. */
    std::chrono::high_resolution_clock::time_point _tp_start;

    /** @brief Reference to the output stream for logging. */
    std::ostream &_log;

    /** @brief Mutex for accessing the log. */
    std::mutex _log_lock;

    /** @brief Thread object for the frame grabber. */
    std::thread _frame_grabber;

    /** @brief Shutdown selector for the frame grabber and callback worksers. */
    volatile bool _shutdown;

    /**
     * @brief   Logs a string to the log output stream.
     *
     * @param[in] log   The string to be logged.
     */
    void logString(const std::string log);

    /**
     * @brief   The frame grabber's worker function.
     */
    void frameGrabberWorker();

    /**
     * @brief   The callback sender's worker function.
     */
    void callbackWorker();

public:

    /**
     * @brief   Constructor for the ViconStream.
     *
     * @param[in] hostname   Address to the Vicon server.
     * @param[in] log_output Reference to the log output stream.
     */
    ViconStream(std::string hostname, std::ostream &log_output);

    /**
     * @brief   Destructor handles the graceful exit of the ViconStream.
     */
    ~ViconStream();

    /**
     * @brief   Enables the Vicon stream and starts receiving data.
     *
     * @param[in] enableSegmentData         Request segment data.
     * @param[in] enableMarkerData          Request marker data.
     * @param[in] enableUnlabeledMarkerData Request unlabeled marker data.
     * @param[in] enableDeviceData          Request device data.
     *
     * @return  Returns true if the stream was started correctly.
     */
    bool enableStream(bool enableSegmentData = true,
                      bool enableMarkerData = false,
                      bool enableUnlabeledMarkerData = false,
                      bool enableDeviceData = false,
                      StreamMode::Enum streamMode = StreamMode::ServerPush);

    /**
     * @brief   Disabled the vicon stream.
     */
    void disableStream();

    /**
     * @brief   Register a callback for data received.
     *
     * @param[in] callback  The function to register.
     * @note    Shall be of the form void(const Output_GetFrame).
     *
     * @return  Return the ID of the callback, is used for unregistration.
     */
    unsigned int registerCallback(viconstream_callback callback);

    /**
     * @brief   Unregister a callback from the queue.
     *
     * @param[in] id  The ID supplied from @p registerCallback.
     *
     * @return  Return true if the ID was deleted.
     */
    bool unregisterCallback(const unsigned int id);
};

#endif
