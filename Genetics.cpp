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

City& City::operator= (const City & c)
{
    lat = c.lat;
    lng = c.lng;
    name = c.name;
    return *this;
}


vector<City>& Population::getCities() {
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
        //swap(temp[position], temp[nextPosition]);
    }
    return temp;
}

double Population::get_tour_distance(vector<City> t) {
    //t= allCities;
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

void Population::sortedPrint (vector<City> n) {
    vector<City>::iterator it;
    for (it = n.begin(); it != n.end(); it++) {

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
    for (it = temp.begin(); it < temp.end(); it++)
    {
        (*it).distance = get_tour_distance((*it).getData());

        (*it).fitness = 1/(*it).distance;
    }
    sort (temp.begin(), temp.end());
    it = temp.begin();
    for (it = temp.begin(); it < temp.begin()+10; it++)
    {
        temp1.push_back(*it);
    }
    return temp1;
}

vector<Tour> Population::crossover(vector<Tour> parents) {
    vector<Tour> children;
    Tour childrenData;

    random_device rd;
    mt19937 gen(rd());
//Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<int> dis(0, 100);
    uniform_int_distribution<int> dis2(0, CITIES_IN_TOUR-1);
//    vector<Tour>::iterator pit;
//    vector<City>::iterator cityTemp;
    int a = 0;
    int noChildren = 0;
    while(noChildren < POPULATION_SIZE)
    {   int parentChange = dis2(gen);
        int mutationPercent =dis(gen);
        int childTempCount = 0;
        int b = 0;

        while (childTempCount<CITIES_IN_TOUR-1)
        {
       //     cout << childTempCount << " < " << (CITIES_IN_TOUR-1) <<"No Children "<< noChildren<< endl;
             if (childTempCount == parentChange) {
                 if (a >= PARENT_POOL_SIZE-1){  a = 0;}else{a++;}
             }
            if (b == CITIES_IN_TOUR-1) {
                b = 0;
            }

        //    cout << "parents.size(): " << parents.size() << ", a: " << a << ", getData().size(): " << parents[a].getData().size() << ", b: " << b << endl;
            if (childrenData.contains_city(parents[a].getData()[b].name)) {
         //       cout << " contains" << endl;
            }
            else{
         //       cout << "adding" << endl;
                childrenData.getData().push_back(parents[a].getData()[b]);
                childTempCount++;}
        //    cout << childTempCount << "/" << CITIES_IN_TOUR-1 << endl;
            b++;
        }
       // cout<<"Came out"<<endl;
        /**
        * Mutation
*/
        if (mutationPercent < MUTATION_RATE) {
            mutate(childrenData.getData());//Mutate
        }
        noChildren++;
      //  cout << childrenData.getData().size() << "/" << CITIES_IN_TOUR << endl;
        children.push_back(childrenData);
        childrenData.getData().clear();
    }
   // cout<<"return";
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
    Tour t;
    t.setData(allCities);
    double prevDistance = get_tour_distance(allCities);
    t.distance = prevDistance;
    double prevFitness = determine_fitness(allCities);
    double fitness;
    vector<City> tt;
    Tour tempElite(0, tt);
    vector<Tour> elite;
    for (int j = 0; j < POPULATION_SIZE; j++) {
        allCities = shuffle_cities(allCities);
        Tour t(fitness, allCities);
        t.distance = get_tour_distance(allCities);
        subset.push_back(t);
    }
    for (int i = 0; i < ITERATIONS; i++) {
        //cout << "........ 1 -> " << subset.size() << endl;
        subset = select_parents(subset);
        //printTour(subset);
        //cout << "........ 2 -> " << subset.size() << endl;
        subset = crossover(subset);
        //printTour(subset);
        //cout << "........ 3 -> " << subset.size() << endl;


            elite.push_back(select_parents(subset)[0]);
            cout<<"\nGENERATION " << i << " ELITE "<< elite.size() << endl;
            printCity(elite[i].getData());
            prevDistance = select_parents(subset)[0].distance;

    }

}

 void Population::mutate(vector<City>& c) {
    uniform_int_distribution<int> dis2(0, c.size()-1);
    vector<City>::iterator pit;
    random_device rd;
    mt19937 gen(rd());
    int a = dis2(gen);
    int b = dis2(gen);
    //cout<<c.size()<<" "<<a<<" "<<b<<endl;
    iter_swap(c.begin() + a, c.begin() + b);
}

void Population::printTour(vector<Tour> t) {
    int tourNumber = 1;
    vector<Tour>::iterator it;
    vector<City>::iterator it2;
    for (it = t.begin(); it != t.end(); it++) {
        cout<<"Tour " << tourNumber<<endl;
        for (it2 = (*it).getData().begin(); it2 != (*it).getData().end(); it2++) {

            cout << (*it2).name << ", ";
        }
        cout<<get_tour_distance((*it).getData())<<endl;
        tourNumber++;
    }

}

void Population::printCity(vector<City> t) {
    vector<City>::iterator it2;

        for (it2 = t.begin(); it2 != t.end(); it2++) {

            cout << (*it2).name << ", ";
        }
        cout<<get_tour_distance(t)<<endl;
}