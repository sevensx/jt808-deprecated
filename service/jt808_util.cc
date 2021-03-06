// Copyright 2019 Yuming Meng. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "service/jt808_util.h"

#include <sys/epoll.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "common/jt808_util.h"
#include "bcd/bcd.h"


int EpollRegister(const int &epoll_fd, const int &fd) {
  struct epoll_event ev;
  int ret;
  int flags;

  // important: make the fd non-blocking.
  flags = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);

  ev.events = EPOLLIN;
  // ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = fd;
  do {
      ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
  } while (ret < 0 && errno == EINTR);

  return ret;
}

int EpollUnregister(const int &epoll_fd, const int &fd) {
  int ret;

  do {
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
  } while (ret < 0 && errno == EINTR);

  return ret;
}

bool ReadDevicesList(const char *path, std::list<DeviceNode *> *list) {
  char *result;
  char line[128] = {0};
  const char *flags = ";\0";
  uint32_t u32val;
  std::ifstream ifs;

  ifs.open(path, std::ios::in | std::ios::binary);
  if (ifs.is_open()) {
    std::string str;
    char *remaining;
    while (getline(ifs, str)) {
      DeviceNode *node = new DeviceNode;
      memset(node, 0x0, sizeof (*node));
      memset(line, 0x0, sizeof(line));
      str.copy(line, str.length(), 0);
      result = strtok_r(line, flags, &remaining);
      str = result;
      str.copy(node->phone_num, str.size(), 0);
      result = strtok_r(nullptr, flags, &remaining);
      str = result;
      u32val = 0;
      sscanf(str.c_str(), "%u", &u32val);
      memcpy(node->authen_code, &u32val, 4);
      node->socket_fd = -1;
      list->push_back(node);
    }
    ifs.close();
    if (!list->empty()) return true;
  }
  return false;
}

int SearchStringInList(const std::vector<std::string> &va_vec,
                       const std::string &arg) {
  auto va_it = va_vec.begin();
  while (va_it != va_vec.end()) {
    if (*va_it == arg) {
      break;
    }
    ++va_it;
  }
  return (va_it == va_vec.end() ? 0 : 1);
}

