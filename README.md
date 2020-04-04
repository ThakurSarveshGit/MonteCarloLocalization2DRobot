[![Udacity - Robotics NanoDegree Program](https://s3-us-west-1.amazonaws.com/udacity-robotics/Extra+Images/RoboND_flag.png)](https://www.udacity.com/robotics)

# RoboND-MCL-Lab
Observe the `MCL` in action through the generated images. 

### Compiling the Program
```sh
$ cd /home/workspace/
$ git clone https://github.com/udacity/RoboND-MCL-Lab
$ cd RoboND-MCL-Lab/
$ rm -rf Images/*
$ g++ main.cpp -o app -std=c++11 -I/usr/include/python2.7 -lpython2.7
```

### Running the Program
Before you run the program, make sure the `Images` folder is empty!
```sh
$ ./app
```
Wait for the program to iterate `50` times.

### Generated Images
After running the program, `50` images will be generated in the `Images` folder.
#### Step0
![alt text](Results/Images/Step0.png)
#### Step49
![alt text](Results/Images/Step49.png)

