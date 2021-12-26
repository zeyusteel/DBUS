#!/bin/bash
gdbus-codegen --interface-prefix GDBus.Demo.            \
            --generate-c-code gen                       \
            --c-generate-object-manager                 \
            --c-namespace  namespace                    \
            gen.xml
