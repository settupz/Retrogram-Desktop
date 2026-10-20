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

	// 1. Шапка и поиск
	builder.addSubsectionTitle(rpl::single(u"Шапка и навигация"_q));

	builder.addButton({
		.id = u"retrogram/show_search"_q,
		.title = rpl::single(u"Кнопка поиска в списке чатов"_q),
		.toggled = cfg.showSearch.value(),
		.onClick = [] {
			::Retrogram::Config().showSearch = !::Retrogram::Config().showSearch.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_menu_btn"_q,
		.title = rpl::single(u"Кнопка главного меню (☰)"_q),
		.toggled = cfg.showMainMenuButton.value(),
		.onClick = [] {
			::Retrogram::Config().showMainMenuButton = !::Retrogram::Config().showMainMenuButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_chat_call"_q,
		.title = rpl::single(u"Кнопка звонка в шапке чата"_q),
		.toggled = cfg.showChatCallButton.value(),
		.onClick = [] {
			::Retrogram::Config().showChatCallButton = !::Retrogram::Config().showChatCallButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_chat_search"_q,
		.title = rpl::single(u"Кнопка поиска внутри чата"_q),
		.toggled = cfg.showChatSearchButton.value(),
		.onClick = [] {
			::Retrogram::Config().showChatSearchButton = !::Retrogram::Config().showChatSearchButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_chat_menu"_q,
		.title = rpl::single(u"Меню из трёх точек в чате (⋮)"_q),
		.toggled = cfg.showChatMenuButton.value(),
		.onClick = [] {
			::Retrogram::Config().showChatMenuButton = !::Retrogram::Config().showChatMenuButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addDivider();

	// 2. Боковое выдвижное меню
	builder.addSubsectionTitle(rpl::single(u"Пункты бокового меню (Drawer)"_q));

	builder.addButton({
		.id = u"retrogram/drawer_profile"_q,
		.title = rpl::single(u"Шапка профиля и аккаунты"_q),
		.toggled = cfg.showDrawerProfile.value(),
		.onClick = [] {
			::Retrogram::Config().showDrawerProfile = !::Retrogram::Config().showDrawerProfile.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_contacts"_q,
		.title = rpl::single(u"Контакты"_q),
		.toggled = cfg.showDrawerContacts.value(),
		.onClick = [] {
			::Retrogram::Config().showDrawerContacts = !::Retrogram::Config().showDrawerContacts.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_calls"_q,
		.title = rpl::single(u"Звонки"_q),
		.toggled = cfg.showDrawerCalls.value(),
		.onClick = [] {
			::Retrogram::Config().showDrawerCalls = !::Retrogram::Config().showDrawerCalls.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_saved"_q,
		.title = rpl::single(u"Избранное"_q),
		.toggled = cfg.showDrawerSavedMessages.value(),
		.onClick = [] {
			::Retrogram::Config().showDrawerSavedMessages = !::Retrogram::Config().showDrawerSavedMessages.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_night"_q,
		.title = rpl::single(u"Тумблер «Ночной режим»"_q),
		.toggled = cfg.showDrawerNightMode.value(),
		.onClick = [] {
			::Retrogram::Config().showDrawerNightMode = !::Retrogram::Config().showDrawerNightMode.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/drawer_settings"_q,
		.title = rpl::single(u"Стандартные «Настройки»"_q),
		.toggled = cfg.showDrawerSettings.value(),
		.onClick = [] {
			::Retrogram::Config().showDrawerSettings = !::Retrogram::Config().showDrawerSettings.current();
			::Retrogram::Config().save();
		},
	});

	builder.addDivider();

	// 3. Чат и строка ввода
	builder.addSubsectionTitle(rpl::single(u"Чат и строка ввода"_q));

	builder.addButton({
		.id = u"retrogram/show_emoji"_q,
		.title = rpl::single(u"Кнопка эмодзи и стикеров"_q),
		.toggled = cfg.showEmojiButton.value(),
		.onClick = [] {
			::Retrogram::Config().showEmojiButton = !::Retrogram::Config().showEmojiButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_attach"_q,
		.title = rpl::single(u"Кнопка вложений (скрепка)"_q),
		.toggled = cfg.showAttachButton.value(),
		.onClick = [] {
			::Retrogram::Config().showAttachButton = !::Retrogram::Config().showAttachButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_voice"_q,
		.title = rpl::single(u"Кнопка голосовых/видео сообщений"_q),
		.toggled = cfg.showVoiceButton.value(),
		.onClick = [] {
			::Retrogram::Config().showVoiceButton = !::Retrogram::Config().showVoiceButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_bot_cmds"_q,
		.title = rpl::single(u"Кнопка команд бота (/)"_q),
		.toggled = cfg.showBotCommandsButton.value(),
		.onClick = [] {
			::Retrogram::Config().showBotCommandsButton = !::Retrogram::Config().showBotCommandsButton.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_avatars"_q,
		.title = rpl::single(u"Аватарки собеседников в группах"_q),
		.toggled = cfg.showAvatarsInGroups.value(),
		.onClick = [] {
			::Retrogram::Config().showAvatarsInGroups = !::Retrogram::Config().showAvatarsInGroups.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_names"_q,
		.title = rpl::single(u"Имена авторов над сообщениями"_q),
		.toggled = cfg.showSenderNames.value(),
		.onClick = [] {
			::Retrogram::Config().showSenderNames = !::Retrogram::Config().showSenderNames.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_reactions"_q,
		.title = rpl::single(u"Реакции под сообщениями"_q),
		.toggled = cfg.showReactions.value(),
		.onClick = [] {
			::Retrogram::Config().showReactions = !::Retrogram::Config().showReactions.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/show_date_pill"_q,
		.title = rpl::single(u"Плашки дат между сообщениями"_q),
		.toggled = cfg.showDatePill.value(),
		.onClick = [] {
			::Retrogram::Config().showDatePill = !::Retrogram::Config().showDatePill.current();
			::Retrogram::Config().save();
		},
	});

	builder.addDivider();

	// 4. Геометрия бабблов (Telegram X Style)
	builder.addSubsectionTitle(rpl::single(u"Стиль сообщений (Telegram X)"_q));

	builder.addButton({
		.id = u"retrogram/bubble_tails"_q,
		.title = rpl::single(u"Хвостики у бабблов сообщений"_q),
		.label = cfg.showBubbleTails.value() | rpl::map([](bool on) {
			return on ? u"Классические"_q : u"Выкл (Капсулы TG X)"_q;
		}),
		.toggled = cfg.showBubbleTails.value(),
		.onClick = [] {
			::Retrogram::Config().showBubbleTails = !::Retrogram::Config().showBubbleTails.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/bubble_radius"_q,
		.title = rpl::single(u"Скругление углов бабблов"_q),
		.label = cfg.bubbleCornerRadius.value() | rpl::map([](int r) {
			return QString::number(r) + u" px"_q;
		}),
		.onClick = [] {
			// Cycle through radii: 4 -> 8 -> 12 -> 16 (TG X) -> 20 -> 24 -> 4
			auto cur = cfg.bubbleCornerRadius.current();
			cur = (cur >= 24) ? 4 : (cur + 4);
			cfg.bubbleCornerRadius = cur;
			cfg.save();
		},
	});

	builder.addDivider();

	// 5. Чистый режим (Анти-Реклама и Premium)
	builder.addSubsectionTitle(rpl::single(u"Чистый режим"_q));

	builder.addButton({
		.id = u"retrogram/hide_premium"_q,
		.title = rpl::single(u"Скрыть промо-кнопки покупки Telegram Premium"_q),
		.toggled = cfg.hidePremiumUpsell.value(),
		.onClick = [] {
			::Retrogram::Config().hidePremiumUpsell = !::Retrogram::Config().hidePremiumUpsell.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/hide_ads"_q,
		.title = rpl::single(u"Блокировать рекламные сообщения в каналах"_q),
		.toggled = cfg.hideSponsoredMessages.value(),
		.onClick = [] {
			::Retrogram::Config().hideSponsoredMessages = !::Retrogram::Config().hideSponsoredMessages.current();
			::Retrogram::Config().save();
		},
	});

	builder.addButton({
		.id = u"retrogram/relocate_stars"_q,
		.title = rpl::single(u"Убрать Telegram Stars с главного экрана"_q),
		.toggled = cfg.relocateStarsToAdvanced.value(),
		.onClick = [] {
			::Retrogram::Config().relocateStarsToAdvanced = !::Retrogram::Config().relocateStarsToAdvanced.current();
			::Retrogram::Config().save();
		},
	});

	builder.addDivider();

	// 6. Звёзды (Скрытый доступ)
	builder.addSubsectionTitle(rpl::single(u"Telegram Stars (Баланс)"_q));
	const auto sessionController = builder.controller();
	builder.addButton({
		.id = u"retrogram/open_stars"_q,
		.title = rpl::single(u"Открыть кошелёк звёзд Telegram Stars"_q),
		.onClick = [=] {
			if (sessionController) {
				sessionController->showSettings(CreditsId());
			}
		},
	});

	builder.addDivider();

	// 7. Сброс к эталону Telegram X
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

const auto kMeta = BuildHelper({
	.id = Retrogram::Id(),
	.parentId = MainId(),
	.title = &tr::lng_settings_advanced, // Fallback icon/meta
	.icon = &st::menuIconSettings,
}, [](SectionBuilder &builder) {
	BuildRetrogramContent(builder);
});

} // namespace

Type RetrogramId() {
	return Retrogram::Id();
}

} // namespace Settings
