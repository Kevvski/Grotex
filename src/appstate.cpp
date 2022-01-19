#include "appstate.h"

AppState::State AppState::state = AppState::State::EMPLOYEES;

AppState::AppState()
{
}


void AppState::setState(const State &newState){
    state = newState;
}


const AppState::State& AppState::getState(){
    return state;
}
