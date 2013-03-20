{
    'targets': [
        {
            'target_name': 'hdf5',
            'include_dirs': [
                '/usr/local/hdf5/include'
            ],
            'sources': [
                'src/hdf5.cc',
                'src/h5_file.cc',
                'src/h5_group.cc',
                'src/h5_dataSet.cc'
            ],
            'link_settings': {
                'libraries': [
                    '-lhdf5',
                    '-lhdf5_cpp'
                ],
                'ldflags': [
                    '-L/usr/local/hdf5/lib'
                ]
            }
        }
    ]
}
