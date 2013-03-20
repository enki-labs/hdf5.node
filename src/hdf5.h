#include <node.h>
#include <string>

#include "H5Cpp.h"

namespace NodeHDF5 {

    using namespace v8;
    using namespace node;
    
    class File : public ObjectWrap {
        
        public:
            static void Initialize (Handle<Object> target);
            H5::H5File* FileObject();
            
        private:
            H5::H5File* m_file;
            File(const char* path); ~File();
            static Handle<Value> New (const Arguments& args);
            static Handle<Value> OpenGroup (const Arguments& args);
            static Handle<Value> OpenDataSet (const Arguments& args);
        
    };
    
    class Group : public ObjectWrap {
        
        private:
            H5::Group m_group;
            static Persistent<Function> Constructor;
            static Handle<Value> New (const Arguments& args);
            
        public:
            Group(H5::Group group);
            static void Initialize ();
            static Handle<Value> Instantiate (const char* path, Local<Object> file);
        
    };

    class DataSet : public ObjectWrap {
        
        private:
            H5::DataSet m_dataSet;
            static Persistent<Function> Constructor;
            static Handle<Value> New (const Arguments& args);
            
        public:
            DataSet(H5::DataSet dataSet);
            H5::DataSet& DataSetObject();
            static void Initialize ();
            static Handle<Value> Instantiate (const char* path, Local<Object> file);            
            static Handle<Value> OpenDataSpace (const Arguments& args);
            static Handle<Value> ReadSync (const Arguments& args);
        
    };

    class DataSpace : public ObjectWrap {
        
        private:
            H5::DataSpace m_dataSpace;
            static Persistent<Function> Constructor;
            static Handle<Value> New (const Arguments& args);
            
        public:
            DataSpace(H5::DataSpace dataSpace);
            static void Initialize ();
            static Handle<Value> Instantiate (Local<Object> file);
            static Handle<Value> GetDimensions (const Arguments& args);
            
    };

};
