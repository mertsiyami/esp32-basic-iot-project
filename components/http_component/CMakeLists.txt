idf_component_register(
    SRCS http_component.c
    INCLUDE_DIRS .
    REQUIRES esp_http_client json
)