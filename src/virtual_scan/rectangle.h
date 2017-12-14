#ifndef VIRTUAL_SCAN_RECTANGLE_H
#define VIRTUAL_SCAN_RECTANGLE_H

#include "line.h"

#include <vector>

namespace virtual_scan
{

struct Rectangle
{
	/** @brief Rectangle pose. */
	Pose pose;

	/** @brief Rectangle corners. */
	std::vector<PointXY> corners;

	/** @brief Rectangle sides. */
	std::vector<Line> sides;

	/**
	 * @brief Default constructor.
	 */
	Rectangle();

	/**
	 * @brief Create a rectangle of given dimensions and pose.
	 *
	 * Position is relative to the center of the rectangle, and orientation is
	 * relative to the length sides.
	 */
	Rectangle(double width, double length, const Pose &pose);
};

} // namespace virtual_scan

#endif
