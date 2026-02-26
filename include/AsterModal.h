#pragma once

#include "AsterGlobal.h"
#include <QDialog>
#include <QString>
#include <functional>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;

namespace AsterFlorets {

    class AsterButton;

    class AsterFlorets_EXPORT AsterModal : public QDialog {
        Q_OBJECT
    public:
        enum class Type {
            Info,
            Success,
            Warning,
            Error,
            Confirm
        };
        Q_ENUM(Type)

        explicit AsterModal(QWidget* parent = nullptr);
        explicit AsterModal(const QString& title, const QString& content, QWidget* parent = nullptr);
        ~AsterModal();

        void setTitle(const QString& title);
        void setContent(const QString& content);
        void setContentWidget(QWidget* widget);

        // Buttons configuration
        void setOkText(const QString& text);
        void setCancelText(const QString& text);
        
        // Static convenience methods
        static bool confirm(QWidget* parent, const QString& title, const QString& content, const QString& okText = "OK", const QString& cancelText = "Cancel");
        static void alert(QWidget* parent, const QString& title, const QString& content, Type type = Type::Info);

    protected:
        void paintEvent(QPaintEvent* event) override;
        
        // Drag support
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void showEvent(QShowEvent* event) override;
        void hideEvent(QHideEvent* event) override;
        void closeEvent(QCloseEvent* event) override;
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        void initUI();
        void updateTheme();
        void showOverlay();
        void hideOverlay();

    private:
        QVBoxLayout* m_mainLayout;
        QWidget* m_headerContainer;
        QLabel* m_titleLabel;
        QWidget* m_bodyContainer;
        QLabel* m_contentLabel; // Default text content
        QWidget* m_customContent;
        QWidget* m_footerContainer;
        AsterButton* m_btnOk;
        AsterButton* m_btnCancel;
        
        QString m_title;
        QString m_content;

        // Dragging
        bool m_isDragging = false;
        QPoint m_dragPosition;

        // Dimmer Overlay
        QWidget* m_overlay = nullptr;
    };

}
