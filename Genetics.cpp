#include "Genetics.hpp"

bool Tour::operator < (const Tour & m) const
{
    //TODO implement the operator
    return (distance < m.distance);
}

bool Tour::operator > (const Tour & m) const
{
    //TODO implement the operator
    return (distance > m.distance);
}


vector<City> Population::getCities() {
    return this->allCities;
}

vector<City> Population::shuffle_cities(vector<City> t) {
    vector<City> temp = t;
    random_device rd;
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<int> dis(0, CITIES_IN_TOUR-1);
    for (int i = 0; i < SHUFFLES; i++) {
        int position = dis(gen);
        int nextPosition = dis(gen);
        iter_swap(temp.begin() + position, temp.begin() + nextPosition);
    }
    return temp;
}

double Population::get_tour_distance(vector<City> t) {
    t= allCities;
    vector<City>::iterator it;
    double distance = 0;

    City temp = *(t.begin());

    for (it = t.begin()+1; it != t.end(); it++) {
        distance+=get_distance_between_cities(temp, *it);
        temp = *(it);
    }
    distance+=get_distance_between_cities(temp, *(t.begin()));
    return distance;
}

void Population::readCities(string fileName) {
    ifstream ifile(fileName, ios::in);
    string temp;
    string line;
    if (!ifile.is_open()) {
        cerr << "There was a problem opening the input file!\n";
    }
    int lat = 0;
    int lng = 0;
    string name = "";
    string word = "";
    CITIES_IN_TOUR = 0;
    while (getline(ifile, line)) {
        istringstream iss(line);
        lat = 0;
        lng = 0;
        name = "";
        word = "";
        int count = 0;
        while(iss >> word) {
            if (count == 0) {
                name = word;
            }
            if (count == 1) {
                lat = stoi(word);
            }
            if (count == 2) {
                lng = stoi(word);
            }
            count++;
        }
        City c(lat, lng, name);
        allCities.push_back(c);
        CITIES_IN_TOUR++;
    }
}

void Population::sortedPrint(vector<City> n) {
    vector<City>::iterator it;

    for (it = n.begin(); it != n.end(); it++)
    {
        cout<<(*it).name<<" : "<<(*it).lat<<" "<<(*it).lng;
        cout<<endl;
    }
}

double Population::get_distance_between_cities(City c1, City c2) {
    const double x_diff = c1.lat - c2.lat;
    const double y_diff = c1.lng - c2.lng;
    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

double Population::determine_fitness(vector<City> t) {
    return 1/get_tour_distance(t);
}

vector<Tour> Population::select_parents(vector<Tour> temp) {
    vector<Tour>::iterator it;
    vector<Tour> temp1;
    for (it = temp.begin(); it != temp.end(); it++)
    {
        (*it).distance = get_tour_distance((*it).getData());

        (*it).fitness = 1/(*it).distance;
    }
    sort (temp.begin(), temp.end());
    for (it = temp.begin(); it != temp.begin()+5; it++)
    {
        temp1.push_back(*it);
    }

    return temp1;
}

vector<Tour> Population::crossover(vector<Tour> parents) {
    vector<Tour> children;
    vector<City> childrenData;

    parents = select_parents(parents);

    random_device rd;
    mt19937 gen(rd());
//Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<int> dis(0, 100);
    uniform_int_distribution<int> dis2(0, CITIES_IN_TOUR-1);
    cout<<"Test"<<endl;
//    vector<Tour>::iterator pit;
//    vector<City>::iterator cityTemp;

    for (auto pit = parents.begin(); pit != parents.end(); pit++)
    {   int parentChange = dis2(gen);
        int mutationPercent =dis(gen);
        int childTempCount = 0;
        cout<<"w\n"<<endl;
        for (auto cityTemp = (*pit).getData().begin(); cityTemp != (*pit).getData().end(); cityTemp++)
        {
            cout<<(*cityTemp).name<<endl;
            if (childTempCount == parentChange) {
                pit++;
            }
            if (childTempCount >= CITIES_IN_TOUR-1) {
                break;
            }
            cout<<(*cityTemp).name<<endl;
            childrenData.push_back(*cityTemp);
            childTempCount++;
        }
        /**
        * Mutation
        */
        if (mutationPercent < MUTATION_RATE) {
            mutate(childrenData);//Mutate
        }
        Tour tt(0, childrenData);
        sortedPrint(childrenData);
        children.push_back(tt);
    }
    printTour(children);
    return children;
}

bool Tour::contains_city(string cityName){
    vector<City>::iterator it;

    for (it = data.begin(); it != data.end(); it++)
    {
       if ((*it).name == cityName)
           return true;
    }
    return false;
}

void Population::run(){
    sortedPrint(allCities);
    double prev = determine_fitness(allCities);
    double fitness = prev;
    vector<City> tt;
    Tour tempElite(0, tt);
    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < POPULATION_SIZE; j++) {
            allCities = shuffle_cities(allCities);
            fitness = determine_fitness(allCities);
            if (fitness > prev) {
                Tour t(fitness, allCities);
                tempElite.setData(allCities);
                prev = determine_fitness(allCities);
                elite.push_back(tempElite);
            }
        }
    }
    crossover(elite);
}

 void Population::mutate(vector<City> c) {
    uniform_int_distribution<int> dis2(0, CITIES_IN_TOUR-1);
    vector<City>::iterator pit;
    random_device rd;
    mt19937 gen(rd());
    int a = dis2(gen);
    int b = dis2(gen);
    iter_swap(c.begin() + a, c.begin() + b);
}

void Population::printTour(vector<Tour> t) {
    vector<Tour>::iterator it;
    for (it = t.begin(); it != t.end(); it++)
    {
        sortedPrint((*it).getData());
    }
}