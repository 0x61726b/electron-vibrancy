{
    "targets": [
        {
            "target_name": "Vibrancy",
            "sources": [
                "src/Common.h",
                "src/Vibrancy.h",
                "src/Vibrancy.cc",
                "src/VibrancyHelper.h",
                "src/vibrancy_win.cc",
                "src/vibrancy_mac.cc",
                "src/vibrancy_linux.cc",
                "src/Init.cc",
            ],
            'conditions':[
                ['OS=="mac"', {
                  'xcode_settings': {
                    'MACOSX_DEPLOYMENT_TARGET': '10.8'
                    }
                }]
            ],
            "variables":{
                "CURRENT_DIR":"<!(echo %~dp0)"
            },
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
