/*
 * Copyright 2014-2019 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

/*
 * Railroad Diagram HTML5 Output
 *
 * Output a HTML page of inlined SVG diagrams
 */

#define _BSD_SOURCE

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "../ast.h"
#include "../xalloc.h"

#include "../rrd/rrd.h"
#include "../rrd/pretty.h"
#include "../rrd/node.h"
#include "../rrd/rrd.h"
#include "../rrd/list.h"
#include "../rrd/tnode.h"

#include "io.h"

/* xxx */
void svg_render_rule(const struct tnode *node, const char *base);
void svg_dim_string(const char *s, unsigned *w, unsigned *a, unsigned *d);

void
html_output(const struct ast_rule *grammar)
{
	const struct ast_rule *p;

	printf("<html DOcTYPE>\n");
	printf("\n");

	printf(" <head>\n");

	printf("  <style>\n");
	printf("      rect, line, path { stroke-width: 1.5px; stroke: black; fill: transparent; }\n");
	printf("      rect, line, path { stroke-linecap: square; stroke-linejoin: rounded; }\n");
	printf("      path { fill: transparent; }\n");
	printf("      text.literal { font-family: monospace; }\n");
	printf("      a { fill: blue; }\n");
	printf("      a:hover rect { fill: aliceblue; }\n");
	printf("      h2 { font-size: inherit; font-weight: inherit; }\n");
	printf("      line.ellipsis { stroke-dasharray: 4; }\n");
	printf("      path.arrow.rtl { marker-mid: url(#rrd:arrow-rtl); }\n");
	printf("      path.arrow.ltr { marker-mid: url(#rrd:arrow-ltr); }\n");
	printf("  </style>\n");
	printf("\n");

	printf(" <svg height='0' width='0' style='float: left'>\n"); /* XXX: why does this take space? */
	printf("  <defs>\n");
	printf("    <marker id='rrd:start'\n");
	printf("        markerWidth='10' markerHeight='12'\n");
	printf("        markerUnits='userSpaceOnUse'\n");
	printf("        refX='7' refY='6'\n");
	printf("        orient='auto'>\n"); /* TODO: auto-start-reverse in SVG2 */
	printf("      <line x1='7' y1='0' x2='7' y2='12' class='arrow'/>\n");
	printf("      <line x1='2' y1='0' x2='2' y2='12' class='arrow'/>\n");
	printf("    </marker>\n");
	printf("\n");

	printf("    <marker id='rrd:arrow-ltr'\n");
	printf("        markerWidth='5' markerHeight='5'\n");
	printf("        refX='3' refY='2.5'\n");
	printf("        orient='auto'>\n");
	printf("      <polyline points='0,0 5,2.5 0,5' class='arrow'/>\n");
	printf("    </marker>\n");
	printf("    <marker id='rrd:arrow-rtl'\n");
	printf("        markerWidth='5' markerHeight='5'\n");
	printf("        refX='3' refY='2.5'\n");
	printf("        orient='auto'>\n");
	printf("      <polyline points='5,0 0,2.5 5,5' class='arrow'/>\n");
	printf("    </marker>\n");
	printf("  </defs>\n");
	printf(" </svg>\n");
	printf("\n");

	printf(" </head>\n");
	printf("\n");

	printf(" <body>\n");

	for (p = grammar; p; p = p->next) {
		struct tnode *tnode;
		struct node *rrd;
		unsigned h, w;

		if (!ast_to_rrd(p, &rrd)) {
			perror("ast_to_rrd");
			return;
		}

		if (prettify) {
			rrd_pretty(&rrd);
		}

		tnode = rrd_to_tnode(rrd, svg_dim_string);

		node_free(rrd);

		printf(" <section>\n");
		printf("  <h2><a name='%s'>%s:</a></h2>\n",
			p->name, p->name);

		h = tnode->a + tnode->d + 2;
		w = tnode->w + 7;

		printf("  <svg height='%u0' width='%u0' viewBox='-20 -10 %u0 %u0'>\n",
			h, w, w, h);
		svg_render_rule(tnode, "");
		printf("  </svg>\n");

		printf(" </section>\n");
		printf("\n");

		tnode_free(tnode);
	}

	printf(" </body>\n");
	printf("</html>\n");
}
