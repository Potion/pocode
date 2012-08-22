//
//  BasicWorker.h
//  poThreadCenterExample
//
//  Created by Steve Varga on 8/22/12.
//
//

#ifndef __poThreadCenterExample__BasicWorker__
#define __poThreadCenterExample__BasicWorker__

#include <iostream>
#include "poThreadCenterWorker.h"

class BasicWorker : public poThreadCenterWorker {
public:
    BasicWorker();
    ~BasicWorker();
    
    void workerFunc(); //All Thread related activity takes place in here
};

#endif /* defined(__poThreadCenterExample__BasicWorker__) */
