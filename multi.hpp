#pragma once

class MultiEngine { /* singleton */
    private:
        MultiEngine() {}
        MultiEngine(const MultiEngine&);
        MultiEngine& operator=(const MultiEngine&);
    public:
        static MultiEngine& getInstance()
        {
            static MultiEngine instance;
            return instance;
        }
        void onBodystateChange(int subtype, VEC pos);
        void onItemuseMessage(int subtype, VEC pos);
};
