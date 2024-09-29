# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Determina a versão mínima do CMAKE suportada pelo projeto
cmake_minimum_required(VERSION 3.12)

# O script 'pico_sdk_import.cmake' é o responsável pela configuração
# dos compiladores que devem ser utilizados bem como os caminhos para
# as bibliotecas para que o software seja devidamente linkado
include(pico_sdk_import.cmake)

# Diretiva padrão utilizada para definir o nome do projeto e as 
# tecnologias utilizadas C,C++ e Assembly
project(i2c_lcd C CXX ASM)

# Configura os Padrões de C e C++ para serem utilizados no projeto
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Configura a placa alvo da compilação
set(PICO_BOARD pico)

# Inicializa o SDK da raspberry pi pico 
pico_sdk_init()

# Marca todos os arquivos fontes e o nome que o binário executável 
# terá no final
add_executable(i2c_lcd
        main.c
        )

# Define as bibliotecas necessárias para que o programa seja linkado
target_link_libraries(i2c_lcd pico_stdlib hardware_i2c)

target_link_libraries(i2c_lcd pico_stdlib hardware_adc)

#configura a saída padrão como sendo uma 'Serial-Over USB'
pico_enable_stdio_usb(i2c_lcd 0)
pico_enable_stdio_uart(i2c_lcd 1)

# Cria os arquivos binários no forma uf2 após a compilação
pico_add_extra_outputs(i2c_lcd)

