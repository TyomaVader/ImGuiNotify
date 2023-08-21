/**
 * @file ImGuiNotify.hpp
 * @brief A header-only library for creating toast notifications with ImGui.
 * 
 * Based on imgui-notify by patrickcjk
 * https://github.com/patrickcjk/imgui-notify
 * 
 * @version 0.0.1 by TyomaVader
 * @date 28.07.2023
 */

#ifndef IMGUI_NOTIFY
#define IMGUI_NOTIFY

#pragma once

#include <vector>
#include <string>
#include <chrono>

#if __cplusplus > 202002L && __cpp_concepts >= 202002L
#include <expected>
#endif

#include "imgui.h"
#include "imgui_internal.h"

#include "IconsFontAwesome6.h"
#include "fa_solid_900.h"









/**
 * CONFIGURATION SECTION Start
*/

#define NOTIFY_MAX_MSG_LENGTH			4096		// Max message content length
#define NOTIFY_PADDING_X				20.f		// Bottom-left X padding
#define NOTIFY_PADDING_Y				20.f		// Bottom-left Y padding
#define NOTIFY_PADDING_MESSAGE_Y		10.f		// Padding Y between each message
#define NOTIFY_FADE_IN_OUT_TIME			150			// Fade in and out duration
#define NOTIFY_DEFAULT_DISMISS			3000		// Auto dismiss after X ms (default, applied only of no data provided in constructors)
#define NOTIFY_OPACITY					0.8f		// 0-1 Toast opacity
#define NOTIFY_USE_SEPARATOR 			false 		// If true, a separator will be rendered between the title and the content
#define NOTIFY_USE_DISMISS_BUTTON		true		// If true, a dismiss button will be rendered in the top right corner of the toast


/**
 * CONFIGURATION SECTION End
*/








static ImGuiWindowFlags NOTIFY_TOAST_FLAGS	=	ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;

#define NOTIFY_NULL_OR_EMPTY(str)		(!str || !strlen(str))
#define NOTIFY_FORMAT(fn, format, ...)	if (format) {va_list args; va_start(args, format); fn(format, args, ##__VA_ARGS__); va_end(args);}

enum class ImGuiToastType : uint8_t
{
	None,
	Success,
	Warning,
	Error,
	Info,
	COUNT
};

enum class ImGuiToastPhase : uint8_t
{
	FadeIn,
	Wait,
	FadeOut,
	Expired,
	COUNT
};

enum class ImGuiToastPos : uint8_t
{
	TopLeft,
	TopCenter,
	TopRight,
	BottomLeft,
	BottomCenter,
	BottomRight,
	Center,
	COUNT
};

/**
 * @brief A class for creating toast notifications with ImGui.
 */
class ImGuiToast
{
private:
	ImGuiToastType								type = ImGuiToastType::None;
	char										title[NOTIFY_MAX_MSG_LENGTH];
	char										content[NOTIFY_MAX_MSG_LENGTH];
	int											dismissTime = NOTIFY_DEFAULT_DISMISS;
	std::chrono::system_clock::time_point		creationTime = std::chrono::system_clock::now();

private:
	// Setters

	inline void setTitle(const char* format, va_list args)
	{
		vsnprintf(this->title, sizeof(this->title), format, args);
	}

	inline void setContent(const char* format, va_list args)
	{
		vsnprintf(this->content, sizeof(this->content), format, args);
	}

public:

	/**
	 * @brief Set the title of the toast notification.
	 * 
	 * @param format The format string for the title.
	 * @param ... The arguments for the format string.
	 */
	inline void setTitle(const char* format, ...)
	{
		NOTIFY_FORMAT(this->setTitle, format);
	}

	/**
	 * @brief Set the content of the toast notification.
	 * 
	 * @param format The format string for the content.
	 * @param ... The arguments for the format string.
	 */
	inline void setContent(const char* format, ...)
	{
		NOTIFY_FORMAT(this->setContent, format);
	}

	/**
	 * @brief Set the type of the toast notification.
	 * 
	 * @param type The type of the toast notification.
	 */
	inline void setType(const ImGuiToastType& type)
	{
		IM_ASSERT(type < ImGuiToastType::COUNT);
		this->type = type;
	};

public:
	// Getters

	/**
	 * @brief Get the title of the toast notification.
	 * 
	 * @return const char* The title of the toast notification.
	 */
	inline const char* getTitle()
	{
		return this->title;
	};

	/**
	 * @brief Get the default title of the toast notification based on its type.
	 * 
	 * @return const char* The default title of the toast notification.
	 */
	inline const char* getDefaultTitle()
	{
		if (!strlen(this->title))
		{
			switch (this->type)
			{
			case ImGuiToastType::None:
				return nullptr;
			case ImGuiToastType::Success:
				return "Success";
			case ImGuiToastType::Warning:
				return "Warning";
			case ImGuiToastType::Error:
				return "Error";
			case ImGuiToastType::Info:
				return "Info";
			default:
				return nullptr;
			}
		}

		return this->title;
	};

	/**
	 * @brief Get the type of the toast notification.
	 * 
	 * @return ImGuiToastType The type of the toast notification.
	 */
	inline ImGuiToastType getType()
	{
		return this->type;
	};

	/**
	 * @brief Get the color of the toast notification based on its type.
	 * 
	 * @return ImVec4 The color of the toast notification.
	 */
	inline ImVec4 getColor()
	{
		switch (this->type)
		{
		case ImGuiToastType::None:
			return {255, 255, 255, 255}; // White
		case ImGuiToastType::Success:
			return {0, 255, 0, 255}; // Green
		case ImGuiToastType::Warning:
			return {255, 255, 0, 255}; // Yellow
		case ImGuiToastType::Error:
			return {255, 0, 0, 255}; // Error
		case ImGuiToastType::Info:
			return {0, 157, 255, 255}; // Blue
		default:
			return {255, 255, 255, 255}; // White
		}
	}

	/**
	 * @brief Get the icon of the toast notification based on its type.
	 * 
	 * @return const char* The icon of the toast notification.
	 */
	inline const char* getIcon() 
	{
		switch (this->type)
		{
		case ImGuiToastType::None:
			return nullptr;
		case ImGuiToastType::Success:
			return ICON_FA_CIRCLE_CHECK; // Font Awesome 6
		case ImGuiToastType::Warning:
			return ICON_FA_TRIANGLE_EXCLAMATION; // Font Awesome 6
		case ImGuiToastType::Error:
			return ICON_FA_CIRCLE_EXCLAMATION; // Font Awesome 6
		case ImGuiToastType::Info:
			return ICON_FA_CIRCLE_INFO; // Font Awesome 6
		default:
			return nullptr;
		}
	}

	/**
	 * @brief Get the content of the toast notification.
	 * 
	 * @return char* The content of the toast notification.
	 */
	inline char* getContent() 
	{
		return this->content;
	};

	/**
	 * @brief Get the elapsed time in milliseconds since the creation of the object.
	 * 
	 * @return int64_t The elapsed time in milliseconds.
	 * @throws An exception with the message "Unsupported platform" if the platform is not supported.
	 */
	inline std::chrono::nanoseconds getElapsedTime() 
	{
		return std::chrono::system_clock::now() - this->creationTime;
	}

	/**
	 * @brief Get the current phase of the toast notification based on the elapsed time since its creation.
	 * 
	 * @return ImGuiToastPhase The current phase of the toast notification.
	 *         - ImGuiToastPhase::FadeIn: The notification is fading in.
	 *         - ImGuiToastPhase::Wait: The notification is waiting to be dismissed.
	 *         - ImGuiToastPhase::FadeOut: The notification is fading out.
	 *         - ImGuiToastPhase::Expired: The notification has expired and should be removed.
	 */
	inline ImGuiToastPhase getPhase()
	{
		const int64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(getElapsedTime()).count();

		if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismissTime + NOTIFY_FADE_IN_OUT_TIME)
		{
			return ImGuiToastPhase::Expired;
		} else 
		if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismissTime)
		{
			return ImGuiToastPhase::FadeOut;
		} else 
		if (elapsed > NOTIFY_FADE_IN_OUT_TIME)
		{
			return ImGuiToastPhase::Wait;
		} else
		{
			return ImGuiToastPhase::FadeIn;
		}
	}

	/**
	 * Returns the percentage of fade for the notification.
	 * @return The percentage of fade for the notification.
	 */
	inline float getFadePercent()
	{
		const ImGuiToastPhase phase = getPhase();
		const int64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(getElapsedTime()).count();

		if (phase == ImGuiToastPhase::FadeIn)
		{
			return ((float)elapsed / (float)NOTIFY_FADE_IN_OUT_TIME) * NOTIFY_OPACITY;
		} else 
		if (phase == ImGuiToastPhase::FadeOut)
		{
			return (1.f - (((float)elapsed - (float)NOTIFY_FADE_IN_OUT_TIME - (float)this->dismissTime) / (float)NOTIFY_FADE_IN_OUT_TIME)) * NOTIFY_OPACITY;
		}

		return 1.f * NOTIFY_OPACITY;
	}

public:
	// Constructors

	/**
	 * @brief Creates a new ImGuiToast object with the specified type and dismiss time.
	 * 
	 * @param type The type of the toast.
	 * @param dismissTime The time in milliseconds after which the toast should be dismissed. Default is NOTIFY_DEFAULT_DISMISS.
	 */
	ImGuiToast(ImGuiToastType type, int dismissTime = NOTIFY_DEFAULT_DISMISS)
	{
		IM_ASSERT(type < ImGuiToastType::COUNT);

		this->type = type;
		this->dismissTime = dismissTime;

		this->creationTime = std::chrono::system_clock::now();

		memset(this->title, 0, sizeof(this->title));
		memset(this->content, 0, sizeof(this->content));
	}

	/**
	 * @brief Constructor for creating an ImGuiToast object with a specified type and message format.
	 * 
	 * @param type The type of the toast message.
	 * @param format The format string for the message.
	 */
	ImGuiToast(ImGuiToastType type, const char* format, ...) : ImGuiToast(type)
	{
		NOTIFY_FORMAT(this->setContent, format);
	}

	/**
	 * @brief Constructor for creating a new ImGuiToast object with a specified type, dismiss time, and content format.
	 * 
	 * @param type The type of the toast message.
	 * @param dismissTime The time in milliseconds before the toast message is dismissed.
	 * @param format The format string for the content of the toast message.
	 * @param ... The variable arguments to be formatted according to the format string.
	 */
	ImGuiToast(ImGuiToastType type, int dismissTime, const char* format, ...) : ImGuiToast(type, dismissTime)
	{
		NOTIFY_FORMAT(this->setContent, format);
	}
};

namespace ImGui
{
	inline std::vector<ImGuiToast> notifications;

	/**
	 * Inserts a new notification into the notification queue.
	 * @param toast The notification to be inserted.
	 */
	inline void InsertNotification(const ImGuiToast& toast)
	{
		notifications.push_back(toast);
	}

	/**
	 * @brief Removes a notification from the list of notifications.
	 * 
	 * @param index The index of the notification to remove.
	 */
	inline void RemoveNotification(int index)
	{
		notifications.erase(notifications.begin() + index);
	}


	/**
	 * Renders all notifications in the notifications vector.
	 * Each notification is rendered as a toast window with a title, content and an optional icon.
	 * If a notification is expired, it is removed from the vector.
	 */
	inline void RenderNotifications()
	{
		const ImVec2 mainWindowSize = GetMainViewport()->Size;

		float height = 0.f;

		for (size_t i = 0; i < notifications.size(); i++)
		{
			ImGuiToast* currentToast = &notifications[i];

			// Remove toast if expired
			if (currentToast->getPhase() == ImGuiToastPhase::Expired)
			{
				RemoveNotification(i);
				continue;
			}

			// Get icon, title and other data
			const char* icon = currentToast->getIcon();
			const char* title = currentToast->getTitle();
			const char* content = currentToast->getContent();
			const char* defaultTitle = currentToast->getDefaultTitle();
			const float opacity = currentToast->getFadePercent(); // Get opacity based of the current phase

			// Window rendering
			ImVec4 textColor = currentToast->getColor();
			textColor.w = opacity;

			// Generate new unique name for this toast
			char windowName[50];
			#ifdef _WIN32
				sprintf_s(windowName, "##TOAST%d", (int)i);
			#elif defined(__linux__)
				sprintf(windowName, "##TOAST%d", (int)i);
			#else
				throw "Unsupported platform";
			#endif

			//PushStyleColor(ImGuiCol_Text, textColor);
			SetNextWindowBgAlpha(opacity);

			// Set notification window position to bottom right corner of the main window, considering the main window size and location in relation to the display
			ImVec2 mainWindowPos = GetMainViewport()->Pos;
			SetNextWindowPos(ImVec2(mainWindowPos.x + mainWindowSize.x - NOTIFY_PADDING_X, mainWindowPos.y + mainWindowSize.y - NOTIFY_PADDING_Y - height), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

			if (!NOTIFY_USE_DISMISS_BUTTON)
			{
				NOTIFY_TOAST_FLAGS |= ImGuiWindowFlags_NoInputs;
			}

			Begin(windowName, nullptr, NOTIFY_TOAST_FLAGS);

			// Render over all other windows
			BringWindowToDisplayFront(GetCurrentWindow());

			// Here we render the toast content
			{
				PushTextWrapPos(mainWindowSize.x / 3.f); // We want to support multi-line text, this will wrap the text after 1/3 of the screen width

				bool wasTitleRendered = false;

				// If an icon is set
				if (!NOTIFY_NULL_OR_EMPTY(icon))
				{
					//Text(icon); // Render icon text
					TextColored(textColor, "%s", icon);
					wasTitleRendered = true;
				}

				// If a title is set
				if (!NOTIFY_NULL_OR_EMPTY(title))
				{
					// If a title and an icon is set, we want to render on same line
					if (!NOTIFY_NULL_OR_EMPTY(icon))
						SameLine();

					Text("%s", title); // Render title text
					wasTitleRendered = true;
				} else 
				if (!NOTIFY_NULL_OR_EMPTY(defaultTitle))
				{
					if (!NOTIFY_NULL_OR_EMPTY(icon))
						SameLine();

					Text("%s", defaultTitle); // Render default title text (ImGuiToastType_Success -> "Success", etc...)
					wasTitleRendered = true;
				}

				// If a dismiss button is enabled
				if (NOTIFY_USE_DISMISS_BUTTON)
				{
					// If a title or content is set, we want to render the button on the same line
					if (wasTitleRendered || !NOTIFY_NULL_OR_EMPTY(content))
					{
						SameLine();
					}

					// Render the dismiss button on the top right corner
					// NEEDS TO BE REWORKED
					float scale = 0.8f;

					if (CalcTextSize(content).x > GetWindowContentRegionMax().x)
					{
						scale = 0.95f;
					}

					SetCursorPosX(GetCursorPosX() + (GetWindowSize().x - GetCursorPosX()) * scale);

					// If the button is pressed, we want to remove the notification
					if (Button(ICON_FA_XMARK))
					{
						RemoveNotification(i);
					}
				}

				// In case ANYTHING was rendered in the top, we want to add a small padding so the text (or icon) looks centered vertically
				if (wasTitleRendered && !NOTIFY_NULL_OR_EMPTY(content))
				{
					SetCursorPosY(GetCursorPosY() + 5.f); // Must be a better way to do this!!!!
				}

				// If a content is set
				if (!NOTIFY_NULL_OR_EMPTY(content))
				{
					if (wasTitleRendered)
					{
						#if NOTIFY_USE_SEPARATOR
							Separator();
						#endif
					}

					Text("%s", content); // Render content text
				}

				PopTextWrapPos();
			}

			// Save height for next toasts
			height += GetWindowHeight() + NOTIFY_PADDING_MESSAGE_Y;

			// End
			End();
		}
	}
}

#endif
