#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#include "Neuron.h"
#include "normal.h"

#include "../CmdLineArgs.h"
// NeuralNetworkLevels;

extern int BBDebug;
extern CmdLineArgs cmdLineArgs;

class Layers{
public:
	Layers(std::string name, int num) :
		_name(name),
		numNeurons(num) {
		neurons = std::vector<Neuron>(num);
	}

	std::string _name;
	int numNeurons;
	std::vector<Neuron> neurons;
};

class NeuralNetwork{
public:
	NeuralNetwork() : 
		_numLayers(0),
		_layers() { }
	// ~NeuralNetwork();

	void load(std::string filename);
	void save(std::string filename) {
		std::fstream fid(filename);

		if(!fid){
			std::cout << "Cannot open File!" << std::endl;
			return;
		}

		std::cout << "Levels: " << this->_numLayers << std::endl;

		for(int i = 0; i < _numLayers; i++){
			std::string name = " ";
			if(i == 0) name = "Output";
			else if(i != _numLayers) name = "Hidden";
			else name = "Input";

			std::cout << "Level: " << name << layer(i).numNeurons << std::endl;

			for(int j = 0; j < layer(i).numNeurons; j++){
				layer(i).neurons[i].saveBaseInfo(fid);

				// BB new if (i == 0) cout << fid << '\n';
				if(i > 0){
					for(int k = 0; k < layer(i).neurons[i].numOfOutputSynapses; k++){
						// Synapse synapse = layer(i).neurons[j].outputSynapse[k];
					}
				}
			}
		}
	}

	void assignInputs(std::vector<int>& inputs);
	void feedForward();
	double computeOutputError(std::vector<double>& trainingVals);
	void backPropagate(double learningRate);

	int numLayers() { return _numLayers; }
	Layers layer(int level) { return _layers[level]; }

private:
	int _numLayers;
	std::vector<Layers> _layers;

	friend std::ostream& operator<<(std::ostream& outs, NeuralNetwork& net);
};
