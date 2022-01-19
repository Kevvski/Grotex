#ifndef APPSTATE_H
#define APPSTATE_H


class AppState
{
public:
    AppState();

    enum class State{
        COMPANY,
        EMPLOYEES,
        CUSTOMERS,
        DATABASES,
        CASH
    };

    enum class ContractState{
        ADD_JOB,
        ADD_ORDER,
        ADD_WORK,
        JOB_ANNEX,
        ORDER_ANNEX,
        WORK_ANNEX,
        NONE
    };

    static void setState(const State& newState);
    static const State& getState();

private:
    static State state;
};

#endif // APPSTATE_H
