/*
Retrogram Customization Engine
Copyright (C) 2026 Retrogram Team
*/
#include "core/retrogram_settings.h"
#include "core/application.h"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

namespace Retrogram {

Settings::Settings() {
	load();
}

Settings &Settings::Instance() {
	static Settings instance;
	return instance;
}

QString Settings::configFilePath() const {
	return cWorkingDir() + u"tdata/retrogram_settings.json"_q;
}

void Settings::resetToTelegramXDefaults() {
	showSearch = true;
	showMainMenuButton = true;
	showChatCallButton = true;
	showChatSearchButton = true;
	showChatMenuButton = true;
	navLayout = NavigationLayout::TelegramXTabs;

	showDrawerProfile = true;
	showDrawerNewGroup = true;
	showDrawerNewChannel = true;
	showDrawerContacts = true;
	showDrawerCalls = true;
	showDrawerSavedMessages = true;
	showDrawerWallet = false;
	showDrawerSettings = true;
	showDrawerNightMode = true;

	showEmojiButton = true;
	showAttachButton = true;
	showVoiceButton = true;
	showBotCommandsButton = true;
	showAvatarsInGroups = true;
	showSenderNames = true;
	showReactions = true;
	showDatePill = true;

	hidePremiumUpsell = true;
	hideSponsoredMessages = true;
	relocateStarsToAdvanced = true;

	bubbleCornerRadius = 16;
	showBubbleTails = false;

	customColorsEnabled = true;
	customBgColor = QString::fromUtf8("#17212b");
	customInBubbleColor = QString::fromUtf8("#232e3c");
	customOutBubbleColor = QString::fromUtf8("#4a7ba5");
	customAccentColor = QString::fromUtf8("#54a9eb");
	customInTextColor = QString::fromUtf8("#f5f5f5");
	customOutTextColor = QString::fromUtf8("#ffffff");

	save();
}

void Settings::load() {
	if (_loaded) {
		return;
	}
	_loaded = true;

	QFile file(configFilePath());
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	const auto doc = QJsonDocument::fromJson(file.readAll());
	if (!doc.isObject()) {
		return;
	}
	const auto obj = doc.object();

	auto readBool = [&](const char *key, rpl::variable<bool> &var) {
		if (obj.contains(key)) {
			var = obj.value(key).toBool(var.current());
		}
	};
	auto readInt = [&](const char *key, rpl::variable<int> &var) {
		if (obj.contains(key)) {
			var = obj.value(key).toInt(var.current());
		}
	};
	auto readString = [&](const char *key, rpl::variable<QString> &var) {
		if (obj.contains(key)) {
			var = obj.value(key).toString(var.current());
		}
	};

	readBool("showSearch", showSearch);
	readBool("showMainMenuButton", showMainMenuButton);
	readBool("showChatCallButton", showChatCallButton);
	readBool("showChatSearchButton", showChatSearchButton);
	readBool("showChatMenuButton", showChatMenuButton);

	if (obj.contains("navLayout")) {
		navLayout = static_cast<NavigationLayout>(obj.value("navLayout").toInt(0));
	}

	readBool("showDrawerProfile", showDrawerProfile);
	readBool("showDrawerNewGroup", showDrawerNewGroup);
	readBool("showDrawerNewChannel", showDrawerNewChannel);
	readBool("showDrawerContacts", showDrawerContacts);
	readBool("showDrawerCalls", showDrawerCalls);
	readBool("showDrawerSavedMessages", showDrawerSavedMessages);
	readBool("showDrawerWallet", showDrawerWallet);
	readBool("showDrawerSettings", showDrawerSettings);
	readBool("showDrawerNightMode", showDrawerNightMode);

	readBool("showEmojiButton", showEmojiButton);
	readBool("showAttachButton", showAttachButton);
	readBool("showVoiceButton", showVoiceButton);
	readBool("showBotCommandsButton", showBotCommandsButton);
	readBool("showAvatarsInGroups", showAvatarsInGroups);
	readBool("showSenderNames", showSenderNames);
	readBool("showReactions", showReactions);
	readBool("showDatePill", showDatePill);

	readBool("hidePremiumUpsell", hidePremiumUpsell);
	readBool("hideSponsoredMessages", hideSponsoredMessages);
	readBool("relocateStarsToAdvanced", relocateStarsToAdvanced);

	readInt("bubbleCornerRadius", bubbleCornerRadius);
	readBool("showBubbleTails", showBubbleTails);

	readBool("customColorsEnabled", customColorsEnabled);
	readString("customBgColor", customBgColor);
	readString("customInBubbleColor", customInBubbleColor);
	readString("customOutBubbleColor", customOutBubbleColor);
	readString("customAccentColor", customAccentColor);
	readString("customInTextColor", customInTextColor);
	readString("customOutTextColor", customOutTextColor);
}

void Settings::save() {
	QJsonObject obj;

	obj["showSearch"] = showSearch.current();
	obj["showMainMenuButton"] = showMainMenuButton.current();
	obj["showChatCallButton"] = showChatCallButton.current();
	obj["showChatSearchButton"] = showChatSearchButton.current();
	obj["showChatMenuButton"] = showChatMenuButton.current();
	obj["navLayout"] = static_cast<int>(navLayout.current());

	obj["showDrawerProfile"] = showDrawerProfile.current();
	obj["showDrawerNewGroup"] = showDrawerNewGroup.current();
	obj["showDrawerNewChannel"] = showDrawerNewChannel.current();
	obj["showDrawerContacts"] = showDrawerContacts.current();
	obj["showDrawerCalls"] = showDrawerCalls.current();
	obj["showDrawerSavedMessages"] = showDrawerSavedMessages.current();
	obj["showDrawerWallet"] = showDrawerWallet.current();
	obj["showDrawerSettings"] = showDrawerSettings.current();
	obj["showDrawerNightMode"] = showDrawerNightMode.current();

	obj["showEmojiButton"] = showEmojiButton.current();
	obj["showAttachButton"] = showAttachButton.current();
	obj["showVoiceButton"] = showVoiceButton.current();
	obj["showBotCommandsButton"] = showBotCommandsButton.current();
	obj["showAvatarsInGroups"] = showAvatarsInGroups.current();
	obj["showSenderNames"] = showSenderNames.current();
	obj["showReactions"] = showReactions.current();
	obj["showDatePill"] = showDatePill.current();

	obj["hidePremiumUpsell"] = hidePremiumUpsell.current();
	obj["hideSponsoredMessages"] = hideSponsoredMessages.current();
	obj["relocateStarsToAdvanced"] = relocateStarsToAdvanced.current();

	obj["bubbleCornerRadius"] = bubbleCornerRadius.current();
	obj["showBubbleTails"] = showBubbleTails.current();

	obj["customColorsEnabled"] = customColorsEnabled.current();
	obj["customBgColor"] = customBgColor.current();
	obj["customInBubbleColor"] = customInBubbleColor.current();
	obj["customOutBubbleColor"] = customOutBubbleColor.current();
	obj["customAccentColor"] = customAccentColor.current();
	obj["customInTextColor"] = customInTextColor.current();
	obj["customOutTextColor"] = customOutTextColor.current();

	const auto path = configFilePath();
	QDir().mkpath(QFileInfo(path).absolutePath());
	QFile file(path);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
	}
}

} // namespace Retrogram

// Retrogram Telegram X Build Trigger
