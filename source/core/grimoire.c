#include "malloc.h"

t_zone grimoire[11] = {
	{64, NULL, NULL},
	{96, NULL, NULL},
	{128, NULL, NULL},
	{192, NULL, NULL},
	{256, NULL, NULL},
	{512, NULL, NULL},
	{1024, NULL, NULL},
	{2048, NULL, NULL},
	{4096, NULL, NULL},
	{8192, NULL, NULL},
	{0, NULL, NULL},
};