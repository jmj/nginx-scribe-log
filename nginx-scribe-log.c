/*
 * nginx scribe loger
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static void *ngx_http_scribe_log_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_scribe_log_merge_loc_conf(ngx_conf_t *cf, void *parent,
    void *child);

/* Should connections be persistant? */
typedef struct {
    ngx_str_t       format;     /* Should be an array */
    ngx_str_t       category;   /* Should be an array */
    ngx_str_t       transport;  /* Should be an array */
} ngx_scribe_log_loc_conf_t;

static ngx_command_t ngx_scribe_log_commands[] = {
    {ngx_string("scribe_log_format"),
     NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_2MORE,
     ngx_conf_set_str_slot, /* Will need to a custom jobby */
     NGX_HTTP_LOC_CONF_OFFSET,
     0,
     NULL },

    {ngx_string("scribe_category"),
     NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     1,
     NULL },

     {ngx_string("scribe_transport_type"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      2,
      NULL },

    ngx_null_command
};

static ngx_http_module_t ngx_http_scribe_log_module_ctx = {
    NULL,               /* preconfiguration */
    NULL,               /* postconfiguration */
    NULL,               /* create main config */
    NULL,               /* init main config */
    NULL,               /* create server config */
    NULL,               /* merge server config */
    ngx_http_scribe_log_create_loc_conf,    /* create location conf */
    ngx_http_scribe_log_merge_loc_conf,     /* merge location conf */
};

static void *
ngx_http_scribe_log_create_loc_conf(ngx_conf_t *cf)
{
    ngx_scribe_log_loc_conf_t *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_scribe_log_loc_conf_t));

    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
/*
    conf->format = ngx_string("");
    conf->category = ngx_string("");
    conf->transport = ngx_string("");
*/
    return conf;
}

static char *
ngx_http_scribe_log_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_scribe_log_loc_conf_t *prev = parent;
    ngx_scribe_log_loc_conf_t *conf = child;
    /* ngx_str_t dflt = ngx_string(""); */


    ngx_conf_merge_str_value(conf->format, prev->format, "");
    ngx_conf_merge_str_value(conf->category, prev->category, "");
    ngx_conf_merge_str_value(conf->transport, prev->transport, "");

    /* sanity checks:
        If format is "", then the rest do not matter
     */

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "format: %s", conf->format.data);
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "category: %s",
        conf->category.data);
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "transport: %s",
        conf->transport.data);

    return NGX_CONF_OK;
}

ngx_module_t ngx_http_scribe_log_module = {
    NGX_MODULE_V1,
    &ngx_http_scribe_log_module_ctx,    /* mod context */
    ngx_scribe_log_commands,            /* mod directives */
    NGX_HTTP_MODULE,                    /* mod type */
    NULL,                               /* init master */
    NULL,                               /* init mod */
    NULL,                               /* init proc */
    NULL,                               /* init thread */
    NULL,                               /* exit thread */
    NULL,                               /* exit proc */
    NULL,                               /* exit master */
    NGX_MODULE_V1_PADDING
};

