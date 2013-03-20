#include <node.h>
#include <string>

#include "H5Cpp.h"
#include "hdf5.h"
#include <iostream>

namespace NodeHDF5 {
    
    using namespace v8;
    
    DataSet::DataSet(H5::DataSet dataSet) {
        
        m_dataSet = dataSet;
        
    }
    
    H5::DataSet& DataSet::DataSetObject() {
        
        return m_dataSet;
        
    }
    
    Persistent<Function> DataSet::Constructor;
    
    void DataSet::Initialize () {        
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        
        // set properties
        t->SetClassName(String::New("DataSet"));
        t->InstanceTemplate()->SetInternalFieldCount(1);
        
        SetPrototypeMethod(t, "dataSpace", OpenDataSpace);
        SetPrototypeMethod(t, "readSync", ReadSync);

        Constructor = Persistent<Function>::New(t->GetFunction());
    }
    
    Handle<Value> DataSet::New (const Arguments& args) {
        
        HandleScope scope;

        if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsObject()) {
            
            ThrowException(v8::Exception::SyntaxError(String::New("expected name, file")));
            return scope.Close(Undefined());
            
        }

        String::Utf8Value set_name (args[0]->ToString());
        File* file = ObjectWrap::Unwrap<File>(args[1]->ToObject());

        DataSet* dataSet = new DataSet(file->FileObject()->openDataSet(*set_name));
        dataSet->Wrap(args.This());
        
        // attach various properties
        args.This()->Set(String::NewSymbol("id"), Number::New(dataSet->m_dataSet.getId()));
        
        
        return args.This();
        
    }
    
    Handle<Value> DataSet::Instantiate (const char* name, Local<Object> file) {
        
        HandleScope scope;
        
        // group name and file reference
        Local<Value> argv[2] = {
                
                Local<Value>::New(String::New(name)),
                file
                
        };
        
        // return new group instance
        return scope.Close(Constructor->NewInstance(2, argv));
        
    }

    Handle<Value> DataSet::OpenDataSpace (const Arguments& args) {
        
        HandleScope scope;
        
        // fail out if arguments are not correct
        if (args.Length() != 1 || !args[0]->IsFunction()) {
            
            ThrowException(v8::Exception::SyntaxError(String::New("expected callback")));
            return scope.Close(Undefined());            
        }
        
        // create callback params
        Local<Value> argv[2] = {
                
                Local<Value>::New(Null()),
                Local<Value>::New(DataSpace::Instantiate(args.This()))
                
        };
        
        // execute callback
        Local<Function> callback = Local<Function>::Cast(args[0]);
        callback->Call(Context::GetCurrent()->Global(), 2, argv);
        
        return scope.Close(Undefined());
        
    }

    Handle<Value> DataSet::ReadSync (const Arguments& args) {

        HandleScope scope;

        if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
            
            ThrowException(v8::Exception::SyntaxError(String::New("expected rowCount, rowOffset")));
            return scope.Close(Undefined());            
        }

        DataSet* set = ObjectWrap::Unwrap<DataSet>(args.This());
        Local<Uint32> rowCount = args[0]->ToUint32();
        Local<Uint32> rowOffset = args[1]->ToUint32();

        H5::DataSpace select = set->m_dataSet.getSpace();
        
        hsize_t length[1];
        int dimensions = select.getSimpleExtentDims(length, NULL);

        hsize_t dims[1];
        dims[0] = rowCount->Value();
        dims[1] = length[1];

        hsize_t offset[1];
        offset[0] = rowOffset->Value();
        offset[1] = 0;

        select.selectHyperslab(H5S_SELECT_SET, dims, offset);

        int rank = 2;
        H5::DataSpace bufferSpace(rank, dims, dims);

        double *data = new double[dims[0]*dims[1]];
        set->m_dataSet.read(data, H5::PredType::NATIVE_DOUBLE, bufferSpace, select);

        Handle<Array> returnVals = Array::New(dims[0]);

        for (size_t i = 0; i < dims[0]; ++i)
        {
            Handle<Array> row = Array::New(dims[1]);

            for (size_t j = 0; j < dims[1]; ++j)
            {
                row->Set(j, Number::New(data[i*dims[1] + j]));
            }

            returnVals->Set(i, row);
        }

        delete data;

        return scope.Close(returnVals);
    }

};
