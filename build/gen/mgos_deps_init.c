/* This file is auto-generated by mos build, do not edit! */

#include <stdbool.h>
#include <stdio.h>

#include "common/cs_dbg.h"

#include "mgos_app.h"


extern bool mgos_mongoose_init(void);
extern bool mgos_ca_bundle_init(void);
extern bool mgos_mqtt_init(void);
extern bool mgos_shadow_init(void);
extern bool mgos_aws_init(void);
extern bool mgos_sntp_init(void);
extern bool mgos_azure_init(void);
extern bool mgos_ota_shadow_init(void);
extern bool mgos_wifi_init(void);
extern bool mgos_http_server_init(void);
extern bool mgos_rpc_common_init(void);
extern bool mgos_dash_init(void);
extern bool mgos_gcp_init(void);
extern bool mgos_i2c_init(void);
extern bool mgos_spi_init(void);
extern bool mgos_ili9341_spi_init(void);
extern bool mgos_mjs_init(void);
extern bool mgos_ota_http_client_init(void);
extern bool mgos_ota_http_server_init(void);
extern bool mgos_rpc_azure_init(void);
extern bool mgos_rpc_mqtt_init(void);
extern bool mgos_rpc_service_config_init(void);
extern bool mgos_rpc_service_fs_init(void);
extern bool mgos_rpc_uart_init(void);
extern bool mgos_watson_init(void);

static const struct lib_descr {
  const char *title;
  bool (*init)(void);
} descrs[] = {

  // "mongoose". deps: [ ]
  {
    .title = "mongoose",
    .init = mgos_mongoose_init,
  },

  // "ca_bundle". deps: [ "mongoose" ]
  {
    .title = "ca_bundle",
    .init = mgos_ca_bundle_init,
  },

  // "mqtt". deps: [ "mongoose" ]
  {
    .title = "mqtt",
    .init = mgos_mqtt_init,
  },

  // "shadow". deps: [ "mongoose" ]
  {
    .title = "shadow",
    .init = mgos_shadow_init,
  },

  // "aws". deps: [ "ca-bundle" "mqtt" "shadow" ]
  {
    .title = "aws",
    .init = mgos_aws_init,
  },

  // "sntp". deps: [ ]
  {
    .title = "sntp",
    .init = mgos_sntp_init,
  },

  // "azure". deps: [ "ca-bundle" "mqtt" "sntp" ]
  {
    .title = "azure",
    .init = mgos_azure_init,
  },

  // "ota_shadow". deps: [ "shadow" ]
  {
    .title = "ota_shadow",
    .init = mgos_ota_shadow_init,
  },

  // "wifi". deps: [ "mongoose" ]
  {
    .title = "wifi",
    .init = mgos_wifi_init,
  },

  // "http_server". deps: [ "atca" "ethernet" "mongoose" "wifi" ]
  {
    .title = "http_server",
    .init = mgos_http_server_init,
  },

  // "rpc_common". deps: [ "http-server" ]
  {
    .title = "rpc_common",
    .init = mgos_rpc_common_init,
  },

  // "dash". deps: [ "ota-shadow" "rpc-common" "shadow" ]
  {
    .title = "dash",
    .init = mgos_dash_init,
  },

  // "gcp". deps: [ "ca-bundle" "mqtt" "sntp" ]
  {
    .title = "gcp",
    .init = mgos_gcp_init,
  },

  // "i2c". deps: [ "mongoose" ]
  {
    .title = "i2c",
    .init = mgos_i2c_init,
  },

  // "spi". deps: [ "mongoose" ]
  {
    .title = "spi",
    .init = mgos_spi_init,
  },

  // "ili9341_spi". deps: [ "spi" ]
  {
    .title = "ili9341_spi",
    .init = mgos_ili9341_spi_init,
  },

  // "mjs". deps: [ "mongoose" ]
  {
    .title = "mjs",
    .init = mgos_mjs_init,
  },

  // "ota_http_client". deps: [ "mongoose" ]
  {
    .title = "ota_http_client",
    .init = mgos_ota_http_client_init,
  },

  // "ota_http_server". deps: [ "http-server" "ota-http-client" ]
  {
    .title = "ota_http_server",
    .init = mgos_ota_http_server_init,
  },

  // "rpc_azure". deps: [ "azure" "rpc-common" ]
  {
    .title = "rpc_azure",
    .init = mgos_rpc_azure_init,
  },

  // "rpc_mqtt". deps: [ "mqtt" "rpc-common" ]
  {
    .title = "rpc_mqtt",
    .init = mgos_rpc_mqtt_init,
  },

  // "rpc_service_config". deps: [ "rpc-common" ]
  {
    .title = "rpc_service_config",
    .init = mgos_rpc_service_config_init,
  },

  // "rpc_service_fs". deps: [ "rpc-common" ]
  {
    .title = "rpc_service_fs",
    .init = mgos_rpc_service_fs_init,
  },

  // "rpc_uart". deps: [ "rpc-common" ]
  {
    .title = "rpc_uart",
    .init = mgos_rpc_uart_init,
  },

  // "watson". deps: [ "ca-bundle" "mqtt" ]
  {
    .title = "watson",
    .init = mgos_watson_init,
  },

};

bool mgos_deps_init(void) {
  size_t i;
  for (i = 0; i < sizeof(descrs) / sizeof(struct lib_descr); i++) {
    LOG(LL_DEBUG, ("init %s...", descrs[i].title));
    if (!descrs[i].init()) {
      LOG(LL_ERROR, ("%s init failed", descrs[i].title));
      return false;
    }
  }

  return true;
}
