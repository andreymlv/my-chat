#pragma once

void init_client(const char *ip, const char *port);
void read_message(int socket);
void send_message(int socket);

