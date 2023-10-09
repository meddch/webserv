#include "utils.hpp"


int main(int ac, char **av)
{
    if(ac != 2)
        return 1;
    try {
        ParseConfig conf(av[1]);
        vector<ServerContext> serv = conf.GetConfig();
        for (int i = 0; i < serv.size(); i++)
        {
                cout << serv[i].clientMaxBodySize << "  "
                    << serv[i].root << "  "
                    << serv[i].serverName << "   "
                    << serv[i].address.port << " ";
                if (serv[i].locations.size())
                    for (int j =0 ;j < serv[i].locations.size();j++)
                    {
                        cout << serv[i].locations[j].allowedMethods[0] << "  "
                            << serv[i].locations[j].uri << "   "
                            << serv[i].locations[j].index[0];
                    }
                    cout << endl;
        } 
    }
    catch (...)
    {
        cerr << ":(\n"; 
    }
}