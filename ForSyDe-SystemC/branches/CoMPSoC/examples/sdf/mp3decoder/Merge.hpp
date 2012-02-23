
#ifndef Merge_HPP
#define Merge_HPP

#include <forsyde.hpp>
#include "include/MP3Decoder.h"

using namespace ForSyDe::SDF;
using namespace std;

typedef tuple<
    vector<ChanuleSamples>,
    vector<ChanuleSamples>,
    vector<bool>,
    vector<FrameHeader>,
    vector<ChanuleSamples>,
    vector<ChanuleSamples>
    > MergeType;

class Merge : public sink<MergeType>
{
public:
    Merge(sc_module_name _name) : sink<MergeType>(_name){}
protected:
    void _func(MergeType inp)
    {
        vector<ChanuleSamples>  inp1 = get<0>(inp);
        vector<ChanuleSamples>  inp2 = get<1>(inp);
        vector<bool>            inp3 = get<2>(inp);
        vector<FrameHeader>     inp4 = get<3>(inp);
        vector<ChanuleSamples>  inp5 = get<4>(inp);
        vector<ChanuleSamples>  inp6 = get<5>(inp);
        
#pragma ForSyDe begin Merge_func
        
        /* Main actor code */
        mergeChanules(&inp4[0], &inp1[0], &inp2[0], &inp5[0], &inp6[0], "output");
        
#pragma ForSyDe end
        if (inp3[0]) sc_stop();
    }
};

#endif
