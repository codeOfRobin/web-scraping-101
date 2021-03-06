#ifndef __xp_h__
#define __xp_h__

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#include "debug.h"
#include "misc.h"

#if !defined(LIBXML_XPATH_ENABLED)
#error "your version of libxml is not compiled with xpath support"
#endif

struct xp_context {
    xmlDocPtr document;
    xmlXPathContextPtr context;
};

struct xp {
    int len;
    xmlNodePtr *nodes;

    // private
    xmlXPathObject *_container;
};

static struct xp *_xp_new(xmlXPathObjectPtr container) 
{
    struct xp *list;
    list = malloc(sizeof(struct xp));
    check(list, "malloc failed");

    xmlNodeSetPtr nodeset = container->nodesetval;
    list->_container = container;
    list->len = (nodeset)?(nodeset->nodeNr):0;
    list->nodes = (nodeset)?(nodeset->nodeTab):NULL;

    return list;
error:
    return NULL;
}

void xp_free(struct xp *obj) 
{
    xmlXPathFreeObject(obj->_container);
    free(obj);
}

struct xp_context *xp_context_new(const char *data) 
{
    struct xp_context *obj = malloc(sizeof(struct xp_context));
    check(obj, "malloc failed");

    obj->document = htmlReadMemory(data, strlen(data), "noname.xml", NULL, 0);
    check(obj->document, "unable to parse html document")

    obj->context = xmlXPathNewContext(obj->document);
    check(obj->context, "unable to create xpath evaluation context");

    return obj;

error:
    if (obj->document) xmlFreeDoc(obj->document);
    if (obj) free(obj);
    return NULL;
}

void xp_context_free(struct xp_context *object)
{

    xmlXPathFreeContext(object->context);
    xmlFreeDoc(object->document);
    free(object);
}

int xp_init()
{
    /**
     * don't worry; I don't understand this test macro either.
     * just think of it as one of those code conventions that
     * nobdy understand but everybody follows.
     */
    LIBXML_TEST_VERSION
    xmlInitParser();

    return HN_OK;
}

int xp_cleanup()
{

    xmlCleanupParser();
    return HN_OK;
}

struct xp *xp_exec(struct xp_context *context, const char *xp_expr) 
{
    xmlXPathObjectPtr container = xmlXPathEvalExpression((xmlChar *)xp_expr, context->context);
    check(container, "error evaulating xpath");

    return _xp_new(container);
error:
    return NULL;
}


#endif
