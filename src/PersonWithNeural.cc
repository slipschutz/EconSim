

#include "PersonWithNeural.hh"
#include "FeedFowardNeuralNetImp.hh"
PersonWithNeural::PersonWithNeural(EconomicActorManager *man ) : EconomicActor(man){


  rBrain = new FeedFowardNeuralNetImp();


}

PersonWithNeural::~PersonWithNeural(){
  delete rBrain;

}

ActorActions PersonWithNeural::BeginningOfStep(){

  return ActorActions::None;
}
ActorActions PersonWithNeural::EndOfStep(){

  return ActorActions::None;
}

void         PersonWithNeural::DoStep(){


}
