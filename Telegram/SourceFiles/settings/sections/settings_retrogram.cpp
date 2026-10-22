/*
Retrogram Customization Section
Copyright (C) 2026 Retrogram Team
*/
#include "settings/sections/settings_retrogram.h"
#include "lang/lang_keys.h"

#include "core/retrogram_settings.h"
#include "settings/settings_common_session.h"
#include "settings/settings_builder.h"
#include "settings/sections/settings_credits.h"
#include "settings/sections/settings_main.h"
#include "window/window_session_controller.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/vertical_layout.h"
#include "styles/style_settings.h"
#include "styles/style_menu_icons.h"

namespace Settings {
namespace {

using namespace Builder;

class Retrogram final : public Section<Retrogram> {
public:
	Retrogram(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();
};

class RetrogramCustomization final : public Section<RetrogramCustomization> {
public:
	RetrogramCustomization(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();
};

Retrogram::Retrogram(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

rpl::producer<QString> Retrogram::title() {
	return rpl::single(u"Настройки Retrogram"_q);
}

void BuildRetrogramContent(SectionBuilder &builder) {
	auto &cfg = ::Retrogram::Config();

	// 1. Главный раздел глубокой кастомизации
	builder.addSectionButton({
		.title = rpl::single(u"Кастомизация"_q),
		.targetSection = RetrogramCustomizationId(),
		.icon = { &st::menuIconPalette },
		.keywords = { u"retrogram"_q, u"customization"_q, u"tgx"_q, u"bubbles"_q },
	});

	builder.addDivider();

	// 2. Telegram Stars (Звёзды) - перенесены сюда, чтобы не мозолить глаза
	builder.addSubsectionTitle(rpl::single(u"Telegram Stars"_q));
	const auto sessionController = builder.controller();
	builder.addButton({
		.id = u"retrogram/open_stars"_q,
		.title = rpl::single(u"Мои звёзды (баланс и транзакции)"_q),
		.icon = { &st::menuIconPremium },
		.onClick = [=] {
			if (sessionController) {
				sessionController->showSettings(CreditsId());
			}
		},
	});

	builder.addDivider();

	// 3. Реклама и монетизация
	builder.addSubsectionTitle(rpl::single(u"Реклама и монетизация"_q));

	builder.addButton({
		.id = u"retrogram/hide_ads"_q,
		.title = rpl::single(u"Блокировать рекламные сообщения в каналах"_q),
		.toggled = cfg.hideSponsoredMessages.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.hideSponsoredMessages = !cfg.hideSponsoredMessages.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/hide_premium"_q,
		.title = rpl::single(u"Скрыть промо-кнопки покупки Telegram Premium"_q),
		.toggled = cfg.hidePremiumUpsell.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.hidePremiumUpsell = !cfg.hidePremiumUpsell.current();
			cfg.save();
		},
	});

	builder.addDivider();

	// 4. Сброс настроек
	builder.addSubsectionTitle(rpl::single(u"Сброс"_q));
	builder.addButton({
		.id = u"retrogram/reset_all"_q,
		.title = rpl::single(u"Сбросить всё к эталону Telegram X"_q),
		.onClick = [] {
			::Retrogram::Config().resetToTelegramXDefaults();
		},
	});

	builder.addSkip();
}

void Retrogram::setupContent() {
	const auto container = Ui::CreateChild<Ui::VerticalLayout>(this);

	const SectionBuildMethod buildMethod = [](
			not_null<Ui::VerticalLayout*> container,
			not_null<Window::SessionController*> controller,
			Fn<void(Type)> showOther,
			rpl::producer<> showFinished) {
		auto &lifetime = container->lifetime();
		const auto highlights = lifetime.make_state<HighlightRegistry>();

		auto builder = SectionBuilder(WidgetContext{
			.container = container,
			.controller = controller,
			.showOther = std::move(showOther),
			.isPaused = Window::PausedIn(
				controller,
				Window::GifPauseReason::Layer),
			.highlights = highlights,
		});

		BuildRetrogramContent(builder);

		std::move(showFinished) | rpl::on_next([=] {
			for (const auto &[id, entry] : *highlights) {
				if (entry.widget) {
					controller->checkHighlightControl(
						id,
						entry.widget,
						base::duplicate(entry.args));
				}
			}
		}, lifetime);
	};

	build(container, buildMethod);
	Ui::ResizeFitChild(this, container);
}

// -------------------------------------------------------------
// RetrogramCustomization
// -------------------------------------------------------------

RetrogramCustomization::RetrogramCustomization(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

rpl::producer<QString> RetrogramCustomization::title() {
	return rpl::single(u"Кастомизация"_q);
}

void BuildRetrogramCustomizationContent(SectionBuilder &builder) {
	auto &cfg = ::Retrogram::Config();

	// 1. Список чатов и шапка
	builder.addSubsectionTitle(rpl::single(u"Список чатов и шапка"_q));

	builder.addButton({
		.id = u"retrogram/show_search"_q,
		.title = rpl::single(u"Строка поиска чатов"_q),
		.toggled = cfg.showSearch.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showSearch = !cfg.showSearch.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_menu_btn"_q,
		.title = rpl::single(u"Кнопка главного меню (☰)"_q),
		.toggled = cfg.showMainMenuButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showMainMenuButton = !cfg.showMainMenuButton.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_chat_call"_q,
		.title = rpl::single(u"Кнопка звонка в шапке чата"_q),
		.toggled = cfg.showChatCallButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showChatCallButton = !cfg.showChatCallButton.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_chat_search"_q,
		.title = rpl::single(u"Кнопка поиска внутри чата"_q),
		.toggled = cfg.showChatSearchButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showChatSearchButton = !cfg.showChatSearchButton.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_chat_menu"_q,
		.title = rpl::single(u"Меню из трёх точек в чате (⋮)"_q),
		.toggled = cfg.showChatMenuButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showChatMenuButton = !cfg.showChatMenuButton.current();
			cfg.save();
		},
	});

	builder.addDivider();

	// 2. Боковое выдвижное меню (Drawer)
	builder.addSubsectionTitle(rpl::single(u"Боковое меню (Drawer)"_q));

	builder.addButton({
		.id = u"retrogram/drawer_profile"_q,
		.title = rpl::single(u"Шапка профиля и аккаунты"_q),
		.toggled = cfg.showDrawerProfile.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerProfile = !cfg.showDrawerProfile.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_group"_q,
		.title = rpl::single(u"Создать группу"_q),
		.toggled = cfg.showDrawerNewGroup.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerNewGroup = !cfg.showDrawerNewGroup.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_channel"_q,
		.title = rpl::single(u"Создать канал"_q),
		.toggled = cfg.showDrawerNewChannel.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerNewChannel = !cfg.showDrawerNewChannel.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_contacts"_q,
		.title = rpl::single(u"Контакты"_q),
		.toggled = cfg.showDrawerContacts.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerContacts = !cfg.showDrawerContacts.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_calls"_q,
		.title = rpl::single(u"Звонки"_q),
		.toggled = cfg.showDrawerCalls.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerCalls = !cfg.showDrawerCalls.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_saved"_q,
		.title = rpl::single(u"Избранное"_q),
		.toggled = cfg.showDrawerSavedMessages.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerSavedMessages = !cfg.showDrawerSavedMessages.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_settings"_q,
		.title = rpl::single(u"Стандартные «Настройки» Telegram"_q),
		.toggled = cfg.showDrawerSettings.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerSettings = !cfg.showDrawerSettings.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_night"_q,
		.title = rpl::single(u"Тумблер «Ночной режим»"_q),
		.toggled = cfg.showDrawerNightMode.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDrawerNightMode = !cfg.showDrawerNightMode.current();
			cfg.save();
		},
	});

	builder.addDivider();

	// 3. Чат и строка ввода
	builder.addSubsectionTitle(rpl::single(u"Чат и строка ввода"_q));

	builder.addButton({
		.id = u"retrogram/show_attach"_q,
		.title = rpl::single(u"Кнопка вложений (скрепка)"_q),
		.toggled = cfg.showAttachButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showAttachButton = !cfg.showAttachButton.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_emoji"_q,
		.title = rpl::single(u"Кнопка эмодзи и стикеров"_q),
		.toggled = cfg.showEmojiButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showEmojiButton = !cfg.showEmojiButton.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_voice"_q,
		.title = rpl::single(u"Кнопка голосовых/видео сообщений"_q),
		.toggled = cfg.showVoiceButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showVoiceButton = !cfg.showVoiceButton.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_bot_cmds"_q,
		.title = rpl::single(u"Кнопка команд бота (/)"_q),
		.toggled = cfg.showBotCommandsButton.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showBotCommandsButton = !cfg.showBotCommandsButton.current();
			cfg.save();
		},
	});

	builder.addDivider();

	// 4. Стиль сообщений (Telegram X Style)
	builder.addSubsectionTitle(rpl::single(u"Стиль сообщений (Telegram X)"_q));

	builder.addButton({
		.id = u"retrogram/bubble_tails"_q,
		.title = rpl::single(u"Хвостики у бабблов сообщений"_q),
		.label = cfg.showBubbleTails.value() | rpl::map([](bool on) {
			return on ? u"Классические"_q : u"Выкл (Капсулы TG X)"_q;
		}),
		.toggled = cfg.showBubbleTails.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showBubbleTails = !cfg.showBubbleTails.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/bubble_radius"_q,
		.title = rpl::single(u"Скругление углов бабблов"_q),
		.label = cfg.bubbleCornerRadius.value() | rpl::map([](int r) {
			return QString::number(r) + u" px"_q;
		}),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			auto cur = cfg.bubbleCornerRadius.current();
			cur = (cur >= 24) ? 4 : (cur + 4);
			cfg.bubbleCornerRadius = cur;
			cfg.save();
		},
	});

	builder.addDivider();

	// 5. Отображение в чатах
	builder.addSubsectionTitle(rpl::single(u"Отображение элементов чата"_q));

	builder.addButton({
		.id = u"retrogram/show_avatars"_q,
		.title = rpl::single(u"Аватарки собеседников в группах"_q),
		.toggled = cfg.showAvatarsInGroups.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showAvatarsInGroups = !cfg.showAvatarsInGroups.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_names"_q,
		.title = rpl::single(u"Имена авторов над сообщениями"_q),
		.toggled = cfg.showSenderNames.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showSenderNames = !cfg.showSenderNames.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_reactions"_q,
		.title = rpl::single(u"Реакции под сообщениями"_q),
		.toggled = cfg.showReactions.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showReactions = !cfg.showReactions.current();
			cfg.save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_date_pill"_q,
		.title = rpl::single(u"Плашки дат между сообщениями"_q),
		.toggled = cfg.showDatePill.value(),
		.onClick = [] {
			auto &cfg = ::Retrogram::Config();
			cfg.showDatePill = !cfg.showDatePill.current();
			cfg.save();
		},
	});

	builder.addSkip();
}

void RetrogramCustomization::setupContent() {
	const auto container = Ui::CreateChild<Ui::VerticalLayout>(this);

	const SectionBuildMethod buildMethod = [](
			not_null<Ui::VerticalLayout*> container,
			not_null<Window::SessionController*> controller,
			Fn<void(Type)> showOther,
			rpl::producer<> showFinished) {
		auto &lifetime = container->lifetime();
		const auto highlights = lifetime.make_state<HighlightRegistry>();

		auto builder = SectionBuilder(WidgetContext{
			.container = container,
			.controller = controller,
			.showOther = std::move(showOther),
			.isPaused = Window::PausedIn(
				controller,
				Window::GifPauseReason::Layer),
			.highlights = highlights,
		});

		BuildRetrogramCustomizationContent(builder);

		std::move(showFinished) | rpl::on_next([=] {
			for (const auto &[id, entry] : *highlights) {
				if (entry.widget) {
					controller->checkHighlightControl(
						id,
						entry.widget,
						base::duplicate(entry.args));
				}
			}
		}, lifetime);
	};

	build(container, buildMethod);
	Ui::ResizeFitChild(this, container);
}

const auto kRetrogramMeta = BuildHelper({
	.id = Retrogram::Id(),
	.parentId = nullptr,
	.title = &tr::lng_settings_advanced,
	.icon = &st::menuIconSettings,
}, [](SectionBuilder &builder) {
	BuildRetrogramContent(builder);
});

const auto kCustomizationMeta = BuildHelper({
	.id = RetrogramCustomization::Id(),
	.parentId = Retrogram::Id(),
	.title = &tr::lng_settings_advanced,
	.icon = &st::menuIconPalette,
}, [](SectionBuilder &builder) {
	BuildRetrogramCustomizationContent(builder);
});

} // namespace

Type RetrogramId() {
	return Retrogram::Id();
}

Type RetrogramCustomizationId() {
	return RetrogramCustomization::Id();
}

} // namespace Settings
