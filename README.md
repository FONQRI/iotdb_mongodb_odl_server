# MONGODB_DBAAS

This is a database as a service based on mongodb.
This app created with [Boost c++ libraries](https://www.boost.org/) , [mongo database](https://www.mongodb.com/) and [c++ 2014 standards](https://en.wikipedia.org/wiki/C%2B%2B14) .I'm developing this application only on linux and I don't know if it works on windows too and I don't care too. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* [mongodb  instalation](https://docs.mongodb.com/manual/administration/install-on-linux/)
* [cmake    instalation](https://cmake.org/install/)
* [Boost    instalation](https://www.boost.org/doc/libs/1_61_0/more/getting_started/unix-variants.html)
* [MONGOCXX instalation](http://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/)

### Installing

After installing prerequisites you only need to open project with [Qt creator](http://doc.qt.io/qtcreator/) or any other ide and run it . If you want to use application on a real server you will need to set database host and port in code too .

## Deployment

I'm developing this application to help people that want to use only database on server side like game leader boards , application setting storage and etc.
let me know if you have any problem with this project .
behnamsabaghi@gmail.com



> Modules
- [x] Database
- [x] Server
- [x] Controllers
- [x] core
- [ ] Security
- [ ] Payments
- [ ] Statistics

>TODOs
- [ ] write postman apis
- [ ] comment mising comments
- [ ] complete security
- [ ] Statistics

## Built With

* [mongodb instalation](https://docs.mongodb.com/)
* [cmake](https://cmake.org/documentation/)
* [Boost](https://www.boost.org/doc/)
* [MONGOCXX](http://mongodb.github.io/mongo-cxx-driver/)

## Commenting

I use [doxygen documentation style](http://doxygen.nl/manual.html) for documenting and I specify priority of TODOs with P[0-3] . e.g :

```
// TODO P[0] Fix Most important bug that crash application when user send an empty json
// TODO P[1] add important API
// TODO P[2] add an exciting feature 
```

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/FONQRI/UCCP_Server/tags). 

## Authors

* **Behnam Sabaghi** - *Initial work* - [FONQRI](https://github.com/FONQRI)


## License

This project is licensed under the GPL V3  License - see the [Licence](LICENSE) file for details

## Acknowledgments

* [Modern Cpp](http://moderncpp.ir/) website 


