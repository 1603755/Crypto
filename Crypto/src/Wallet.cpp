#include "../include/Wallet.h"

Wallet::Wallet() {
	if (!Utils::file_exists(PRIVATE_KEY_PATH) && !Utils::file_exists(PUBLIC_KEY_PATH)) {
		Utils::Keys();
	}
	PublicKey = Utils::getPublicKey();
}
