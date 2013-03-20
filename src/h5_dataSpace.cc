#include <node.h>
#include <string>

#include "H5Cpp.h"
#include "hdf5.h"

namespace NodeHDF5 {
    
    using namespace v8;
    
    DataSpace::DataSpace(H5::DataSpace dataSpace) {
        
        m_dataSpace = dataSpace;
        
    }
    
    
    Persistent<Function> DataSpace::Constructor;
    
    void DataSpace::Initialize () {
        
        // instantiate constructor template
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        
        // set properties
        t->SetClassName(String::New("DataSpace"));
        t->InstanceTemplate()->SetInternalFieldCount(1);

        SetPrototypeMethod(t, "getDimensions", GetDimensions);
        
        
        // initialize constructor reference
        Constructor = Persistent<Function>::New(t->GetFunction());
        
    }
    
    Handle<Value> DataSpace::New (const Arguments& args) {
        
        HandleScope scope;
        
        // fail out if arguments are not correct
        if (args.Length() != 1 || !args[0]->IsObject()) {
            
            ThrowException(v8::Exception::SyntaxError(String::New("expected name, file")));
            return scope.Close(Undefined());
            
        }
        
        // unwrap data set
        DataSet* set = ObjectWrap::Unwrap<DataSet>(args[0]->ToObject());
        
        // create space
        DataSpace* space = new DataSpace(set->DataSetObject().getSpace());
        space->Wrap(args.This());
        
        // attach various properties
        args.This()->Set(String::NewSymbol("ndims"), Number::New(space->m_dataSpace.getSimpleExtentNdims()));

        return args.This();
        
    }
    
    Handle<Value> DataSpace::Instantiate (Local<Object> dataSet) {
        
        HandleScope scope;

        Local<Value> argv[1] = {
                dataSet                
        };
        
        // return new group instance
        return scope.Close(Constructor->NewInstance(1, argv));        
    }

    Handle<Value> DataSpace::GetDimensions (const Arguments& args) {
        
        HandleScope scope;

        DataSpace* dataSpace = ObjectWrap::Unwrap<DataSpace>(args.This());
        hsize_t length[1];
        int dimensions = dataSpace->m_dataSpace.getSimpleExtentDims(length, NULL);

        Handle<Object> result = Object::New();
        result->Set(String::NewSymbol("dimensions"), Number::New(dimensions));
        result->Set(String::NewSymbol("rows"), Number::New(length[0]));
        result->Set(String::NewSymbol("columns"), Number::New(length[1]));
        return scope.Close(result);
        
    }

};
