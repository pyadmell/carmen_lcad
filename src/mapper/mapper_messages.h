/*********************************************************
	 ---   My Module Specific Messages ---

See IPC documentation for more information:
http://www.cs.cmu.edu/~ipc/

*********************************************************/
#include <carmen/carmen.h>
#ifndef CARMEN_MAPPER_MESSAGES_H
#define CARMEN_MAPPER_MESSAGES_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	double *complete_map;
	int size;
	carmen_map_config_t config;
	double timestamp;
	char *host;
} carmen_mapper_map_message;

#define CARMEN_MAPPER_MAP_MESSAGE_NAME	"carmen_mapper_map_message"
#define CARMEN_MAPPER_MAP_MESSAGE_FMT		"{<double:2>, int, {int, int, double, [byte:64], string, double, double}, double, string}"
#define CARMEN_MAPPER_MAP_MOVING_OBJECTS_RAW_MAP_MESSAGE_NAME "carmen_mapper_map_moving_objects_raw_map_message_name"


typedef struct
{
	int *coord_x;
	int *coord_y;
	double *value;
	int size;
	carmen_map_config_t config;
	double timestamp;
	char *host;
} carmen_mapper_compact_map_message;

#define CARMEN_MAPPER_COMPACT_MAP_MESSAGE_NAME	"carmen_mapper_compact_map_message"
#define CARMEN_MAPPER_COMPACT_MAP_MESSAGE_FMT		"{<int:4>, <int:4>, <double:4>, int, {int, int, double, [byte:64], string, double, double}, double, string}"


typedef struct
{
	int num_positions;
	carmen_position_t *positions;
	double timestamp;
	char *host;
} carmen_mapper_virtual_laser_message;

#define CARMEN_MAPPER_VIRTUAL_LASER_MESSAGE_NAME	"carmen_mapper_virtual_laser_message"
#define CARMEN_MAPPER_VIRTUAL_LASER_MESSAGE_FMT		"{int,<{double,double}:1>, double, string}"


#ifdef __cplusplus
}
#endif

#endif

// @}
