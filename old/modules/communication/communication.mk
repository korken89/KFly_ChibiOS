# List of all the module's related files.
COMMUNICATION_SRCS = $(MODULE_DIR)/communication/src/circularbuffer.c \
                     $(MODULE_DIR)/communication/src/slip.c \
                     $(MODULE_DIR)/communication/src/cobs.c \
                     $(MODULE_DIR)/communication/src/serialmanager.c \
                     $(MODULE_DIR)/communication/src/subscriptions.c \
                     $(MODULE_DIR)/communication/src/kflypacket_generators.c \
                     $(MODULE_DIR)/communication/src/kflypacket_parsers.c \
                     $(MODULE_DIR)/communication/src/slip2kflypacket.c

# Required include directories
COMMUNICATION_INC = $(MODULE_DIR)/communication/inc
