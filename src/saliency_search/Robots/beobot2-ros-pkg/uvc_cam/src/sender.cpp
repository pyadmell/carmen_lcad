/*
 * Copyright (c) 2009, Morgan Quigley, Clemens Eppner, Tully Foote
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Stanford U. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdio>
#include <ros/ros.h>
#include <ros/time.h>
#include "uvc_cam/uvc_cam.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/image_encodings.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "uvc_cam");
  ros::NodeHandle n;
  ros::NodeHandle n_private("~");

  std::string device;
  std::string out_topic;
  n_private.param<std::string>("device", device, "/dev/video0");
  n_private.param<std::string>("topic", out_topic, "/camera/image");
  int width, height, fps, modetect_lum, modetect_count;
  n_private.param("width", width, 640);
  n_private.param("height", height, 480);
  n_private.param("fps", fps, 30);
  n_private.param("motion_threshold_luminance", modetect_lum, 100); 
  n_private.param("motion_threshold_count", modetect_count, -1); 

  ros::Publisher pub = n.advertise<sensor_msgs::Image>(out_topic.c_str(), 1);
  ROS_INFO("opening uvc_cam at %dx%d, %d fps", width, height, fps);
  uvc_cam::Cam cam(device.c_str(), uvc_cam::Cam::MODE_RGB, width, height, fps);
  cam.set_motion_thresholds(modetect_lum, modetect_count);

  ros::Time t_prev(ros::Time::now());
  int count = 0, skip_count = 0;
  while (n.ok())
  {
    unsigned char *frame = NULL;
    uint32_t bytes_used;
    int buf_idx = cam.grab(&frame, bytes_used);
    if (count++ % fps == 0)
    {
      ros::Time t(ros::Time::now());
      ros::Duration d(t - t_prev);
      ROS_INFO("%.1f fps skip %d", (double)fps / d.toSec(), skip_count);
      t_prev = t;
    }
    if (frame)
    {
      sensor_msgs::Image image; 
      image.header.stamp = ros::Time::now();
      image.encoding = sensor_msgs::image_encodings::RGB8;
      image.height = height;
      image.width = width;
      image.step = 3 * width;

      image.set_data_size( image.step * image.height );
      /*
      uint8_t* bgr = &(image.data[0]);
      for (uint32_t y = 0; y < height; y++)
        for (uint32_t x = 0; x < width; x++)
        {
          // hack... flip bgr to rgb
          uint8_t *p = frame + y * width * 3 + x * 3;
          uint8_t *q = bgr   + y * width * 3 + x * 3;
          q[0] = p[2]; q[1] = p[1]; q[2] = p[0];
        }
      */
      memcpy(&image.data[0], frame, width * height * 3);
      pub.publish(image);
      cam.release(buf_idx);
    }
    else
      skip_count++;
  }
  return 0;
}

