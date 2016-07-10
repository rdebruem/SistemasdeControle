#include "SistemasdeControle/headers/optimizationLibs/recursiveleastsquare.h"

template <class Type>
OptimizationHandler::RecursiveLeastSquare<Type>::RecursiveLeastSquare(ModelHandler::Model<Type> *model,
                                                     Type p0,
                                                     Type lambda)
{
    srand(time(NULL));
    this->p0     = p0;
    this->model  = model;
    this->lambda = lambda;    
    this->firstTimeFlag = false;
    this->model->setModelCoef(LinAlg::Ones<Type>(this->model->getNumberOfVariables(), this->model->getNumberOfOutputs())/this->p0);
//    this->model->setModelCoef(LinAlg::Random<double>(this->model->getNumberOfVariables(), this->model->getNumberOfOutputs())/this->p0);
    this->P = LinAlg::Eye<Type>(this->model->getNumberOfVariables())*(this->p0);

    this->Input  = LinAlg::Zeros<Type>(this->model->getNumberOfInputs(), 2);
    this->Output = LinAlg::Zeros<Type>(this->model->getNumberOfOutputs(), 2);
}

template <class Type>
void OptimizationHandler::RecursiveLeastSquare<Type>::Optimize()
{
}

template <class Type>
void OptimizationHandler::RecursiveLeastSquare<Type>::Optimize(LinAlg::Matrix<Type> Input, LinAlg::Matrix<Type> Output)
{
    this->Input.removeColumn(this->Input.getNumberOfColumns());
    this->Output.removeColumn(this->Output.getNumberOfColumns());
    this->Input = Input|this->Input;
    this->Output = Output|this->Output;
    this->model->setLinearVector(this->Input.GetColumn(1),
                                 this->Output.GetColumn(2));


    LinAlg::Matrix<Type> phi = this->model->getLinearVectorA();
    E = this->Output.GetColumn(1) - ~(phi*this->model->getModelCoef());
    K = (P*~phi)/(((phi*P)*~phi) + lambda);
    LinAlg::Matrix<Type> KxE;
    for(unsigned i = 1; i <= this->model->getNumberOfOutputs(); ++i)
        KxE = KxE | K*E(i,1);
    this->model->setModelCoef(this->model->getModelCoef() + KxE);
    P = (P - (K*(phi*P)))/lambda;
}
