//
// Created by dldms on 11/25/2018.
//

#ifndef ASSINGMENT2CPP_GENETICS_HPP
#define ASSINGMENT2CPP_GENETICS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <fstream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <random>
using namespace std;
/**
 * Genetics algorithm, author: David Lee
 */
/**
 *
 * Class to store the city name and lat and long.
 */
class City {
public:
    int lat;
    int lng;
    string name;
    City(int lat1,int lng1,string name1) {
        lat = lat1;
        lng = lng1;
        name = name1;
    }
    City& operator= (const City & c);
};

/**
 * Tour to store a vector of cities and it can contain its distance and
 * fitness.
 */
class Tour {
private:
    /**
     * Vector of cities.
     */
    vector<City> data;
public:
    /**
     * Operator.
     * @param m
     * @return
     */
    bool operator > (const Tour & m) const;
    /**
     * Operator.
     * @param m
     * @return
     */
    bool operator < (const Tour & m) const;
    /**
     * Default constructor.
     */
    Tour(){

    }

    /**
     * Construct tour with data.
     * @param fitness1
     * @param data1
     */
    Tour(double fitness1, vector<City> data1) {
        fitness = fitness1;
        distance = 1/fitness;
        data = data1;
    }

    /**
     * Get the vector of tour data.
     * @return
     */
    vector<City>& getData() {
        return this->data;
    }
    /**
     * Setter for the data.
     * @param temp
     */
    void setData(vector<City> temp) {
        this->data = temp;
    }

    /**
     * Fitness value of this tour.
     */
    double fitness;
    /**
     * distance value of this tour.
     */
    double distance;

    /**
     * Boolean to determine whether it contains certain city or not.
     * @param cityName
     * @return
     */
    bool contains_city(string cityName);
};

/**
 * Population class to manipulate the tourclasses to
 * calculate the shortest distance tour.
 */
class Population {
private:
    /**
    * Vector of city that is first initialized by the reader method.
    */
    vector<City> allCities;

    /**
     * Temporary subset to determine the parents.
     */
    vector<Tour> subset;

    /**
     * Elite tour that has the least distances.
     */
    vector<Tour> elite;

    /**
     * Temporary vector of Tours for calc purposes.
     */
    vector<Tour> population;

    /**
     * Constants provided.
     */
    int CITIES_IN_TOUR;
    int POPULATION_SIZE = 20;
    int SHUFFLES = 69;
    int ITERATIONS = 1000;
    int MAP_BOUNDARY = 1000;
    int PARENT_POOL_SIZE = 10;
    int MUTATION_RATE = 15;
    int NUMBER_OF_ELITES = 1;

public:
    /**
     * Print the vector data of the tour.
     * @param n
     */
    void sortedPrint(vector<City> n);

    /**
     * Return the initialized cities in the population class.
     * @return
     */
    vector<City>& getCities();

    /**
     * Read cities from the textfile.
     * @param fileName
     */
    void readCities(string fileName);

    /**
     * Shuffle cities in the vector<city>
     * @param t
     * @return
     */
    vector<City> shuffle_cities(vector<City> t);

    /**
     * Print cities.
     * @param t
     */
    void printCity(vector<City> t);

    /**
     * Get distance between cities.
     * @param c1
     * @param c2
     * @return
     */
    double get_distance_between_cities(City c1, City c2);

    /**
     * Get the distance of the tour.
     * @param t
     * @return
     */
    double get_tour_distance(vector<City> t);

    /**
     * Determine the fitness of the tour object data.
     * @param t
     * @return
     */
    double determine_fitness(vector<City> t);

    /**
     * Select parents of the Tour vector and return
     * child vector.
     * @param t
     * @return
     */
    vector<Tour> select_parents(vector<Tour> t);

    /**
     * Combine two parents to make a child.
     * @return
     */
    vector<Tour> crossover(vector<Tour> parents);

    /**
     * Mutate a child.
     * @return
     */
    void mutate(vector<City>& c);

    /**
     * Initial run method.
     */
    void run();

    /**
     * Print the tour.
     * @param t
     */
    void printTour(vector<Tour> t);
};



#endif //ASSINGMENT2CPP_GENETICS_HPP
