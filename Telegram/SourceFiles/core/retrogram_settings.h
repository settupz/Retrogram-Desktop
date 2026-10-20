/*
Retrogram Customization Engine
Copyright (C) 2026 Retrogram Team
*/
#pragma once

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <rpl/variable.h>

namespace Retrogram {

enum class NavigationLayout : int {
	TelegramXTabs = 0,
	BottomBar = 1,
	Minimal = 2,
};

class Settings final {
public:
	static Settings &Instance();

	void load();
	void save();
	void resetToTelegramXDefaults();

	// Navigation & Header
	rpl::variable<bool> showSearch{ true };
	rpl::variable<bool> showMainMenuButton{ true };
	rpl::variable<bool> showChatCallButton{ true };
	rpl::variable<bool> showChatSearchButton{ true };
	rpl::variable<bool> showChatMenuButton{ true };
	rpl::variable<NavigationLayout> navLayout{ NavigationLayout::TelegramXTabs };

	// Drawer Menu
	rpl::variable<bool> showDrawerProfile{ true };
	rpl::variable<bool> showDrawerNewGroup{ true };
	rpl::variable<bool> showDrawerNewChannel{ true };
	rpl::variable<bool> showDrawerContacts{ true };
	rpl::variable<bool> showDrawerCalls{ true };
	rpl::variable<bool> showDrawerSavedMessages{ true };
	rpl::variable<bool> showDrawerWallet{ false };
	rpl::variable<bool> showDrawerSettings{ true };
	rpl::variable<bool> showDrawerNightMode{ true };

	// Chat View & Input Bar
	rpl::variable<bool> showEmojiButton{ true };
	rpl::variable<bool> showAttachButton{ true };
	rpl::variable<bool> showVoiceButton{ true };
	rpl::variable<bool> showBotCommandsButton{ true };
	rpl::variable<bool> showAvatarsInGroups{ true };
	rpl::variable<bool> showSenderNames{ true };
	rpl::variable<bool> showReactions{ true };
	rpl::variable<bool> showDatePill{ true };

	// Monetization & Clean-up
	rpl::variable<bool> hidePremiumUpsell{ true };
	rpl::variable<bool> hideSponsoredMessages{ true };
	rpl::variable<bool> relocateStarsToAdvanced{ true };

	// Bubble Geometry
	rpl::variable<int> bubbleCornerRadius{ 16 };
	rpl::variable<bool> showBubbleTails{ false }; // false = Telegram X smooth capsule bubbles

	// Palette & Colors (HEX)
	rpl::variable<bool> customColorsEnabled{ true };
	rpl::variable<QString> customBgColor{ QString::fromUtf8("#17212b") };
	rpl::variable<QString> customInBubbleColor{ QString::fromUtf8("#232e3c") };
	rpl::variable<QString> customOutBubbleColor{ QString::fromUtf8("#4a7ba5") };
	rpl::variable<QString> customAccentColor{ QString::fromUtf8("#54a9eb") };
	rpl::variable<QString> customInTextColor{ QString::fromUtf8("#f5f5f5") };
	rpl::variable<QString> customOutTextColor{ QString::fromUtf8("#ffffff") };

private:
	Settings();
	~Settings() = default;
	Settings(const Settings &) = delete;
	Settings &operator=(const Settings &) = delete;

	QString configFilePath() const;
	bool _loaded = false;
};

[[nodiscard]] inline Settings &Config() {
	return Settings::Instance();
}

} // namespace Retrogram
