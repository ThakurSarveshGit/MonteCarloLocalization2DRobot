#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <stdexcept>
#include <random>

using namespace std;

// Global Variables
double landmarks[8][2] = {{20,20}, {20,80}, {20, 50}, {50,20}, {50,80},
						  {80,80}, {80,20}, {80,50}};

double world_size = 100.0;

random_device rd;
mt19937 gen(rd());

// Global Functions
double mod(double first_term, double second_term); // Truncation function
double gen_real_random(); // Random Number generator

class Robot{

public:
	double x, y, orient; // robot poses
	double forward_noise, turn_noise, sense_noise; // robot noises

	Robot(){
		// Constructor

		// Generate Robot's Position and Orientation randomly.
		x = gen_real_random()*world_size;
		y = gen_real_random()*world_size;
		orient = gen_real_random()*2.0*M_PI;

		forward_noise = 0.0;
		turn_noise = 0.0;
		sense_noise = 0.0;
	}


	void set(double new_x, double new_y, double new_orient){
		// Set robot new position and orientation
		if (new_x < 0 || new_x >= world_size)
			throw std::invalid_argument("X coordinate out of bound");
		if (new_y < 0 || new_y >= world_size)
			throw std::invalid_argument("Y coordinate out of bound");
		if (new_orient < 0 || new_orient >= 2*M_PI)
			throw std::invalid_argument("Orientation must be in [0..2pi]");

		x = new_x;
		y = new_y;
		orient = new_orient;
	}

	void set_noise(double new_forward_noise, double new_turn_noise, double new_sense_noise){
		// Simulate noise, often useful in particle filters
		forward_noise = new_forward_noise;
		turn_noise = new_turn_noise;
		sense_noise = new_sense_noise;
	}

	vector<double> sense(){
		// Measure the distances from the robot toward the landmarks
		vector<double> z(sizeof(landmarks)/sizeof(landmarks));
		double dist;

		for (int i=0; i<sizeof(landmarks)/sizeof(landmarks); i++){
			dist = sqrt(pow((x-landmarks[i][0]),2) + pow((y-landmarks[i][1]),2));
			dist += gen_gauss_random(0.0, sense_noise);
			z[i] = dist;
		}
		return z;
	}

	Robot move(double turn, double forward){
		if (forward < 0)
			throw std::invalid_argument("Robot cannot move backward!");

		// turn and add randomness to the turning command
		orient = orient + turn + gen_gauss_random(0.0, turn_noise);
		orient = mod(orient, 2*M_PI);

		// move and add randomness to the motion command
		double dist = forward + gen_gauss_random(0.0, forward_noise);
		x = x + (cos(orient) * dist);
		y = y + (sin(orient) * dist);

		// Cyclic Truncation
		x = mod(x, world_size);
		y = mod(y, world_size);

		// Set particle
		Robot res;
		res.set(x, y, orient);
		res.set_noise(forward_noise, turn_noise, sense_noise);

		return res;
	}

    string show_pose(){
        // Returns the robot current position and orientation in a string format
        return "[x=" + to_string(x) + " y=" + to_string(y) + " orient=" + to_string(orient) + "]";
    }

	string read_sensors(){
		// Returns all the distances from the robot towards the landmarks
		vector<double> z = sense();
		string readings = "[";

		for (int i=0; i<z.size(); i++)
			readings += to_string(z[i]) + " ";

		readings[readings.size() - 1] = "]";

		return readings;
	}

	double measurement_prob(vector<double> measurement){
		// Calculates how likely a measurement should be
		double prob = 1.0;
		double dist;

		for (int i=0; i<sizeof(landmarks)/sizeof(landmarks[0]); i++){
			dist = sqrt(pow((x-landmarks[i][0]), 2) + pow((y-landmarks[i][1]), 2));
			prob *= gaussian(dist, sense_noise, measurement[i]);
		}
		return prob;
	}

private:

	double gen_gauss_random(double mean, double variance){
		// Gaussian random
		normal_distribution<double> gauss_dict(mean, variance);
		return gauss_dist(gen);
	}

    double gaussian(double mu, double sigma, double x){
        // Probability of x for 1-dim Gaussian with mean mu and var. sigma
        return exp(-(pow((mu - x), 2)) / (pow(sigma, 2)) / 2.0) / sqrt(2.0 * M_PI * (pow(sigma, 2)));
    }
};

// Functions
double gen_real_random()
{
    // Generate real random between 0 and 1
    uniform_real_distribution<double> real_dist(0.0, 1.0); //Real
    return real_dist(gen);
}

double mod(double first_term, double second_term){
	// Compute the modulus
	return first_term - (second_term)*floor(first_term/(second_term));
}


double evaluation(Robot r, Robot p[], int n){
    //Calculate the mean error of the system
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        //the second part is because of world's cyclicity
        double dx = mod((p[i].x - r.x + (world_size / 2.0)), world_size) - (world_size / 2.0);
        double dy = mod((p[i].y - r.y + (world_size / 2.0)), world_size) - (world_size / 2.0);
        double err = sqrt(pow(dx, 2) + pow(dy, 2));
        sum += err;
    }
    return sum / n;
}

double max(double arr[], int n){
    // Identify the max element in an array
    double max = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}


int main(){
    
    cout << "I am ready for coding the MCL!";

    return 0;

}