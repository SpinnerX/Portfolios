#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <cmath>
#include <cctype>
#include <iomanip>

class Synapse;

class Neuron{
public:
	Neuron() :
		valueInput(0.0),
		valueOutput(0.0),
		valueError(0),
		numOfInputSynapses(-1),
		numOfOutputSynapses(-1) { }

	// virtual ~Neuron(){ }
	
	// refactoring and updating function SaveBaseInfo(FILE* fid);
	virtual void saveBaseInfo(std::fstream& ins) { }
	
	std::string name; // Holds "ftanh" or bias or whatever
	double valueInput;
	double valueOutput;
	double valueError;
	
	int numOfInputSynapses;
	int numOfOutputSynapses; // used only for fanin
	
	virtual double squashingFunction(double val) { return val; }
	virtual double squashingSlope(double val) { return 1.0; }

	virtual int acceptsInput() { return 1; }

	virtual std::ostream& operator<<(std::ostream& outs) { return outs; }
};

class Synapse{
public:
	Synapse() :
		weight(0.0),
		deltaWeight(0.0),
		learningRate(0.0),
		weightNeedsInit(0.0),
		neuronNeedsInit(0.0),
		tempNeuronLevel(-1),
		tempNeuronNumber(-1),
		srcNeuron(),
		destNeuron() { }

	double weight;
	double deltaWeight;
	double learningRate;

	int weightNeedsInit; // if weight needs to be set to sqrt(fanin)
	int neuronNeedsInit; // neuron that has not been seen
	int tempNeuronLevel; // level of destroyed neuron
	int tempNeuronNumber; // number of destroyed neuron

	Neuron srcNeuron;
	Neuron destNeuron;
};

class NeuronBias : public Neuron{
public:
	NeuronBias();

	// virtual ~NeuronBias();

	virtual double squashingFunction(double val) { return 1.0; }
	virtual double squashingSlope(double val) { return 1.0; }

	virtual int acceptsInput() { return 0; }
};

class NeuronThreshold : public Neuron{
public:
	NeuronThreshold() : _threshold(0.5) { }
	NeuronThreshold(double thresh) : _threshold(thresh) { }

	// virtual ~NeuronThreshold();

	virtual double squashingFunction(double val) {
		if(val >= _threshold) return 1.0;
		else return -1.0;
	}

	virtual double squashingSlope(double val) {
		double expA = exp(val);
		double expNegA = exp(-val);

		double g_of_a = ( (expA - expNegA) / (expA + expNegA));

		return (1.0 - g_of_a * g_of_a);
	}

	virtual void saveBaseInfo(std::fstream ins) {
		ins >> name >> numOfOutputSynapses >> _threshold;
	}

	virtual std::ostream& operator<<(std::ostream& outs) {
		outs << "Threshold " << _threshold;
		return outs;
	}

	double _threshold;
};

class NeuronLogistic : public Neuron{
public:
	NeuronLogistic() : _scalar(0.0) { }
	NeuronLogistic(double scaleVal) : _scalar(scaleVal) { }
	
	// virtual ~NeuronLogistic();
	virtual double squashingFunction(double val) { return ((1.0 / (1.0 + exp(-val))) + _scalar * val); }
	virtual double squashingSlope(double val){
		double g_of_a = squashingFunction(val);
		return (_scalar + g_of_a * (1.0 - g_of_a));
	}

	virtual void saveBaseInfo(std::istream ins){
		ins >> name >> numOfOutputSynapses >> _scalar;
	}

	virtual std::ostream& operator<<(std::ostream& outs){
		outs << "Logistic " << _scalar;
		return outs;
	}

	double _scalar;
};

class NeuronTanh : public Neuron{
public:
	NeuronTanh() = default;
	// virtual ~NeuronTanh();

	virtual double squashingFunction(double val) {
		double expA = exp(val);
		double expNegA = exp(-val);

		return ((expA - expNegA) / (expA + expNegA));
	}
	virtual double squashingSlope(double val) {
		double g_of_a = squashingFunction(val);
		return (1.0 - g_of_a * g_of_a);
	}
};

class NeuronFunnyTanh : public NeuronTanh {
public:
	NeuronFunnyTanh() : _scalar(0.001) { }
	NeuronFunnyTanh(double scaleVal) : _scalar(scaleVal) { }
	// virtual ~NeuronFunnyTanh();

	virtual double squashingFunction(double val) {
		static const double TWO_THIRDS = 2.0 / 3.0;
		return (1.7519 * std::tanh(TWO_THIRDS * val) + _scalar * val); 
	}

	virtual double squashingSlope(double val){
		static const double TWO_THIRDS = 2.0 / 3.0;
		double pseudo_g_of_a = std::tanh(TWO_THIRDS * val);
		return (_scalar + 1.7519 * TWO_THIRDS * (1.0 - pseudo_g_of_a * pseudo_g_of_a));
	}

	virtual void saveBaseInfo(std::istream ins){
		ins >> name >> numOfOutputSynapses >> _scalar;
	}

	virtual std::ostream& operator<<(std::ostream& outs){
		outs << "fTanh " << _scalar;
		return outs;
	}

	double _scalar;
};

class NeuronFactory{
public:
	NeuronFactory();
	// ~NeuronFactory();

	Neuron createNeuron(std::string fnStr, double tmpDbl){
		if(fnStr == "logistic"){
			if(tmpDbl == 0.0) return NeuronLogistic();
			else return NeuronLogistic(tmpDbl);
		}
		else if(fnStr == "tanh") return NeuronTanh();
		else if(fnStr == "ftanh"){
			if(tmpDbl == 0.0) return NeuronFunnyTanh();
			else return NeuronFunnyTanh(tmpDbl);
		}
		else if(fnStr == "bias") return NeuronBias();
		else if(fnStr == "threshold") return NeuronThreshold(tmpDbl);
		else return Neuron();
	}
};

std::ostream& operator<<(std::ostream& outs, Neuron& neuron){
	outs.setf(std::ios::fixed);
	outs << "	" << neuron <<  " ";
	outs << neuron << " F( "
		 << std::setw(5) << std::setprecision(5) << neuron.valueInput
		 << ") --> "
		 << std::setw(5) << std::setprecision(5) << neuron.valueOutput
		 << "	==> "
		 << std::setw(50) << std::setprecision(5) << neuron.valueError;

	return outs;
}

std::ostream& operator<<(std::ostream& outs, Synapse& synapse) {
	outs << synapse.destNeuron
		 << " <-- "
		 << std::setw(5) << std::setprecision(5) << synapse.weight
		 << " <-- "
		 << synapse.srcNeuron;
	return outs;
}

std::ostream& operator<<(std::ostream& outs, NeuronLogistic& neuron) {
	outs << "Logistic " << ((Neuron &) neuron);
	return outs;
}

std::ostream& operator<<(std::ostream& outs, NeuronTanh& neuron) {
	outs << "Tanh " << ((Neuron &) neuron);
	return outs;
}

std::ostream& operator<<(std::ostream& outs, NeuronFunnyTanh& neuron){
	outs << "F " << ((NeuronTanh &) neuron);
	return outs;
}
