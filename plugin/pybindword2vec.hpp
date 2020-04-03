
#ifndef _TRT_PROFILER_H_
#define _TRT_PROFILER_H_

#include <fstream>
#include <iostream>

#include"word2vec.hpp"


class CWord2Vec
{
private:
std::unique_ptr<w2v::w2vModel_t> w2vModel;
std::unique_ptr<w2v::d2vModel_t> d2vModel;
// get nearest article IDs from the model
std::vector<std::pair<std::size_t, float>> nearest;
void readFile(const std::string &_fileName, std::string &_data);
public:
//std::unordered_map<long unsigned int, std::vector<float>> getMapData();
//CWord2Vec(const std::string &_fileName); 
int loadWord2Vec(const std::string &_fileName);
int loadDoc2VecModel(const std::string &_fileName);
void saveDoc2VecModel(const std::string outfilename);
int convertDoc2Vecv1(const std::string filename, int id);
std::vector<float> convertDoc2Vec(const std::string filename);
std::vector<std::pair<std::size_t, float>> Doc2VecSimilarity(const std::string filename, int max_nearest_vec);
int myadd(int a , int b){return(a+b);}
    
};

/*CWord2Vec::CWord2Vec(const std::string &_fileName){
   try {
        w2vModel.reset(new w2v::w2vModel_t());
        // load w2v model file
        if (!w2vModel->load(_fileName)) {
            throw std::runtime_error(w2vModel->errMsg());
        }
    } catch (const std::exception &_e) {
        std::cerr << _e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 2;
    }
}*/
int CWord2Vec::loadWord2Vec(const std::string &_fileName){
   try {
        this->w2vModel.reset(new w2v::w2vModel_t());
        // load w2v model file
        if (!w2vModel->load(_fileName)) {
            throw std::runtime_error(w2vModel->errMsg());
        }
         this->d2vModel.reset(new w2v::d2vModel_t(this->w2vModel->vectorSize()));
    } catch (const std::exception &_e) {
        std::cerr << _e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 2;
    }
   return 0;
}

//std::unordered_map<long unsigned int, std::vector<float>> CWord2Vec::getMapData(){
//auto data = this->d2vModel->map();
//return data;
//}

int CWord2Vec::loadDoc2VecModel(const std::string &_fileName){
   try {
        this->d2vModel.reset(new w2v::d2vModel_t(this->w2vModel->vectorSize()));
        // load w2v model file
        if (!d2vModel->load(_fileName)) {
            throw std::runtime_error(w2vModel->errMsg());
        }
    } catch (const std::exception &_e) {
        std::cerr << _e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 2;
    }
   return 0;
}

void CWord2Vec::readFile(const std::string &_fileName, std::string &_data) {
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ifs.open(_fileName.c_str());
    ifs.seekg(0, ifs.end);
    auto size = ifs.tellg();
    _data.resize(static_cast<std::size_t>(size), 0);
    ifs.seekg(0, ifs.beg);
    ifs.read(&_data[0], size);
    ifs.close();
}

void CWord2Vec::saveDoc2VecModel(const std::string outfilename){
   // finaly, save our doc2vec model
        if (!this->d2vModel->save(outfilename)) {
            std::cerr << "Can not save model: " << this->d2vModel->errMsg() << std::endl;
        }
}

int CWord2Vec::convertDoc2Vecv1(const std::string filename, int id)
{       
       
       try {
        std::string fileText ;
        this->readFile(std::string(filename), fileText);
        //  text to vector
        w2v::doc2vec_t doc2vec(this->w2vModel, fileText);
        // add vector with ID = 1 to the model
        this->d2vModel->set(id, doc2vec);
        
    } catch (const std::exception &_e) {
        std::cerr << _e.what() << std::endl;
        return 3;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 3;
    }
     
 return 0;
}

std::vector<float> CWord2Vec::convertDoc2Vec(const std::string filename)
{       
       std::vector<float> data;
       try {
        std::string fileText ;
        this->readFile(std::string(filename), fileText);
        //  text to vector
        w2v::doc2vec_t doc2vec(this->w2vModel, fileText);
        data= static_cast<std::vector<float>> (doc2vec);
        this->d2vModel->set(0, doc2vec);

    } catch (const std::exception &_e) {
        std::cerr << _e.what() << std::endl;
        return data;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return data;
    }
     
 return data;
}

std::vector<std::pair<std::size_t, float>> CWord2Vec::Doc2VecSimilarity(const std::string filename, int max_nearest_vec)
{       
         this->nearest.clear();
         //std::vector<std::pair<std::size_t, float>>
     try {
        std::string fileText ;
        this->readFile(std::string(filename), fileText);
        //  text to vector
        w2v::doc2vec_t doc2vec(this->w2vModel, fileText);
        this->d2vModel->nearest(doc2vec, this->nearest, max_nearest_vec);
        } catch (const std::exception &_e) {
        std::cerr << _e.what() << std::endl;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
    }
      // output result set
        
        for (auto const &i:nearest) {
            std::cout << i.first << ": " << i.second << std::endl;
            
        }
    return this->nearest;
}



#endif //_FULLY_CONNECTED_H
