#include <carmen/carmen.h>
#include <carmen/localize_ackerman_interface.h>
#include <carmen/fused_odometry_interface.h>


carmen_point_t
publish_starting_pose(carmen_point_t pose)
{
	carmen_point_t std;

	std.x = 0.001;
	std.y = 0.001;
	std.theta = carmen_degrees_to_radians(0.01);

	carmen_localize_ackerman_initialize_gaussian_command(pose, std);

	return pose;
}


void
define_messages()
{
	IPC_RETURN_TYPE err;

	carmen_localize_ackerman_define_globalpos_messages();

	err = IPC_defineMsg(CARMEN_FUSED_ODOMETRY_PARTICLE_NAME, IPC_VARIABLE_LENGTH, CARMEN_FUSED_ODOMETRY_PARTICLE_FMT);
	carmen_test_ipc_exit(err, "Could not define", CARMEN_FUSED_ODOMETRY_PARTICLE_NAME);
}


int
main(int argc, char **argv)
{
	double initial_timestamp, time_passed;
	carmen_point_t pose;
	int time = 4;

	if (argc < 4)
	{
		printf("Use %s <x> <y> <theta> <OPTIONAL period>\n "
				"Period the program remains publishing the initial pose\n", argv[0]); // Period the program remains publishing the initial pose
		exit(-1);
	}
	if (argc == 5)
	{
		time = atoi(argv[4]);
	}

	pose.x = atof(argv[1]);
	pose.y = atof(argv[2]);
	pose.theta = atof(argv[3]);

	carmen_ipc_initialize(argc, argv);

	double sleep_time = (10.0) * 10e5;
	initial_timestamp = carmen_get_time();

	while (1)
	{
		time_passed = (carmen_get_time() - initial_timestamp);

		if (time_passed < time)
			publish_starting_pose(pose);
		else
			usleep((int) sleep_time);
	}

	return 0;
}

