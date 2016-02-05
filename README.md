# video_ros<br  />
STEPS: <br  />
1)Open a terminal and type source devel/setup.bash<br  />

2)rosrun image_tansfer video_node 0 (for publisher) <br  />
3)repeat step 1 and then type rosrun image_tansfer videos_node (for subscriber)<br  />

To run the two nodes in multiple computer<br  />
Tegra <br  />
** in /etc/hosts file add ip and the groundstation hostname   **
1)export ROS_HOSTNAME=tegra-ubuntu  <br  />  
2)export ROS_MASTER_URI=http://tegra-ubuntu:11311<br  />
3)export ROS_IP=tegra_ip<br  />


In ground station

** in /etc/hosts file add ip and the tegra hostname   **
1)export ROS_HOSTNAME=tegra-ubuntu  <br  />  
2)export ROS_MASTER_URI=http://tegra's ip:11311<br  />
3)export ROS_IP=ground station ip<br  />
