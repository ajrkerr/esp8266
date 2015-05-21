#include "page_builder.h"

void PageBuilder::write(WiFiClient client, const char data[], int length) {
  for(int i = 0; i < length; i += 2048) {
    int amnt = (length - i < 2048)? length - i : 2048;
    client.write(data + i, amnt);
  }
}

void PageBuilder::javascript(WiFiClient client) {
  write(client, javascriptData, strlen(javascriptData));
}

void PageBuilder::html(WiFiClient client) {
  write(client, htmlData, strlen(htmlData));
}
