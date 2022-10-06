#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

static u_char *_data; /* belongs to set =1  or not =0*/
static u_char *_result; /* belongs to contour =1  or not =0*/
static int _sx, _sy;
static int neighbours[8];
static void init_neighbours() {
	neighbours[0] = 1;
	neighbours[4] = -1;
	neighbours[1] = -_sx+1;
	neighbours[2] = -_sx;
	neighbours[3] = -_sx-1;
	neighbours[5] = _sx-1;
	neighbours[6] = _sx;
	neighbours[7] = _sx+1;
}
	
static void make_contour(int first) { /*Pavlidis  algorithm 7.1 */
	int a=first, b=first, c=first;
	int found = 0, s = 6, count=0;

	_result[first] = 1;
	do {
		while (!found && count<3) {
			b = c+neighbours[s==0? 7 : s-1];
			if (_data[b]) {
				c = b; s = s-2; found = 1;
			} else {
				b = c+neighbours[s];
				if (_data[b]) {
					c = b; found = 1;
				}
				else {
					b = c+neighbours[s+1];
					if (_data[b]) {
						c = b; found = 1;
					} else s = s+2;
				}
			}
			s = (s<0)? (-s)%8 : s%8;
			count++;
		}
		if (found) _result[c] = 1;
		found = count = 0;
	} while (c != first);
}
	
static int find_first() {
	int i=0, size = _sx*_sy;
	for (i = _sx+1; i<size; i++)
		if (_data[i] && !_result[i] && !_data[i-1]) return i;
	return -1;
}

static void  make_contours() {
	u_char *p1, *p2;
	int j=0, first=0, sx = _sx-2;
	_result = (u_char*)calloc(_sx*_sy,1);
	init_neighbours();
	while ((first = find_first()) > 0) {
		make_contour(first);
}
	p1 = _result; p2 = _result+_sx+1;
/* shift 1,1 */
	for (j=1; j < _sy-1; j++) {
		memcpy(p1,p2,sx);
		p1 += sx; p2 += _sx;
	}
}

u_char* greater_contours(short* data, int sx, int sy, short level) {
	int i, j;
	u_char *p1;
	short *p0 = data;
	_sx = sx+2; _sy = sy+2;						/* add 0 boundaries */
	_data = (u_char*)calloc(_sx*_sy,1);
	for (j=0; j<sy; j++) {
		p1 = _data+_sx*j+1;
		for (i=0; i<sx; i++, p1++) 
			if (*p0++ > level) *p1 = 1; 
	}
	make_contours();
	free(_data);
	return _result;
}

u_char* little_contours(short* data, int sx, int sy, short level) {
	int i, j;
	u_char* p1;
	short *p0 = data;
	_sx = sx+2; _sy = sy+2;						/* add 0 boundaries */
	_data = (u_char*)calloc(_sx*_sy,1);
	for (i=0; i<sy; i++) {
		p1 = _data+_sx*j+1;
		for (j=0; j<sx; j++, p1++) 
			if (*p0++ < level) *p1 = 1;
	}
	make_contours();
	free(_data);
	return _result;
}
