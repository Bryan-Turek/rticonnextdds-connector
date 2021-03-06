{
  'variables': {
    'app_name': 'RTI Connext Connector',
    'system': '<!(uname -s)',
    'release': '',
    'rtiLibrary': 'rtiddsprototyper',
    'conditions': [
      ['target_arch=="ia32"', {
        'target_arch': 'i86'
      }],
      ['OS!="win"', {
        'system%': '<!(uname -s)',
        'release%': '<!(uname -r)'
      }],
      ['OS=="mac"', {
      	'libDir%' : "<(module_root_dir)/bin/<(target_arch)<(system)12clang8.0",
        'rtiLib%': "librtiddsconnector.dylib"
      }],
    ]
  },
  'targets': [{
    'target_name': 'rtiddsconnector',
    'sources': [
      'src/rtiddsconnector.cc',
      'src/connector.cc',
      'src/reader.cc',
      'src/writer.cc',
      'src/samples.cc',
    ],
    "cflags_cc!": [ "-fno-rtti", "-fno-exceptions" ],
    "cflags!": [ "-fno-exceptions" ],
    "include_dirs" : [
      "<!(node -e \"require('nan')\")"
    ],
    'conditions': [
      ['OS=="mac"', {
        "xcode_settings": {
          'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++', '-v'],
          'OTHER_LDFLAGS': ['-stdlib=libc++'],
          'MACOSX_DEPLOYMENT_TARGET': '10.7',
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
        },
        'link_settings': {
          'libraries': [
            '-lrtiddsconnector'
          ],
          'library_dirs': [
            '<(module_root_dir)/bin/x64Darwin12clang8.0',
          ],
        },
      }],
      ['OS=="linux"', {
        "link_settings": {
          "libraries": [
            '-lrti_dds_connector'
          ],
          "library_dirs": [
            '<(module_root_dir)/bin/x64Linux2.6gcc4.4.5'
          ]
        }
      }]
    ]
  }]
}
