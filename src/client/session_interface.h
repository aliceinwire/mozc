// Copyright 2010, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// An abstract interface for the session class.

#ifndef MOZC_CLIENT_SESSION_INTERFACE_H_
#define MOZC_CLIENT_SESSION_INTERFACE_H_

namespace mozc {
namespace config {
class Config;
}
namespace commands {
class KeyEvent;
class Output;
class SessionCommand;
}

namespace client {

class SessionInterface {
 public:
  virtual ~SessionInterface() {}

  // return true if the current thread is running
  // inside valid run level.
  virtual bool IsValidRunLevel() const = 0;

  // Returns true if connection is alive.
  // If connection is not available,  re-launch mozc_server internally.
  virtual bool EnsureConnection() = 0;

  // Returns true if session id is valid.
  // if session id is invalid, re-issue a valid sssion id.
  virtual bool EnsureSession() = 0;

  // Checks protocol/product version.
  // If a  new version is avaialable, restart the server.
  // return true the server is available.
  // return false some error happend during the server restart.
  // This method calls EnsureConnection automatically.
  virtual bool CheckVersionOrRestartServer() = 0;

  // SendKey/TestSendKey/SendCommand automatically
  // make a connection and issue an session id
  // if valid session id is not found.
  virtual bool SendKey(const commands::KeyEvent &key,
                       commands::Output *output) = 0;
  virtual bool TestSendKey(const commands::KeyEvent &key,
                           commands::Output *output) = 0;
  virtual bool SendCommand(const commands::SessionCommand &command,
                           commands::Output *output) = 0;

  // The methods below don't call
  // StartServer even if server is not available. This treatment
  // avoids unexceptional and continuous server restart trials.
  // If you really want to ensure the connection,
  // call EnsureConnection() in advance.

  // Get/Set config data
  virtual bool GetConfig(config::Config *config) = 0;
  virtual bool SetConfig(const config::Config &config) = 0;

  // Clear history data
  virtual bool ClearUserHistory() = 0;
  virtual bool ClearUserPrediction() = 0;
  virtual bool ClearUnusedUserPrediction() = 0;

  // Shutdonw server
  virtual bool Shutdown() = 0;

  // Sync server data, e.g, (prediction data) into disk
  virtual bool SyncData() = 0;

  // Reload server data, e.g., (user dictionary, prediction data)
  virtual bool Reload() = 0;

  // Cleanup un-used sessions
  virtual bool Cleanup() = 0;

  // Resets internal state (changs the state to be SERVER_UNKNWON)
  virtual void Reset() = 0;

  // Returns true if server is alive.
  // This method will never change the internal state.
  virtual bool PingServer() const = 0;

  // This method is similar to PingServer(), but the internal
  // state may change. In almost all cases, you don't need to
  // call this method.
  virtual bool NoOperation() = 0;

  // Enables or disables using cascading window.
  virtual void EnableCascadingWindow(bool enable) = 0;

  // Sets the time out in milli second used for the IPC connection.
  virtual void set_timeout(int timeout) = 0;

  // Sets restricted mode.
  // server is launched inside restricted enviroment.
  virtual void set_restricted(bool restricted) = 0;

  // Sets server program path.
  // mainly for unittesting.
  virtual void set_server_program(const string &program_path) = 0;

  // Launches mozc tool. |mode| is the mode of MozcTool,
  // e,g,. "config_dialog", "dictionary_tool".
  virtual bool LaunchTool(const string &mode,
                          const string &extra_arg) = 0;

  // Launches browser and pass |url|
  virtual bool OpenBrowser(const string &url) = 0;
};

class SendCommandInterface {
 public:
  virtual ~SendCommandInterface() {}
  virtual bool SendCommand(const commands::SessionCommand &command,
                           commands::Output *output) = 0;
};
}  // namespace client
}  // namespace mozc
#endif  // MOZC_CLIENT_SESSION_INTERFACE_H_