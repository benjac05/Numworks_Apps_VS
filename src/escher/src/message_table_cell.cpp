#include <escher/message_table_cell.h>
#include <escher/palette.h>
#include <assert.h>

namespace Escher {

MessageTableCell::MessageTableCell(I18n::Message label) :
  TableCell(),
  m_messageTextView(KDFont::Size::Large, label, 0, 0.5, KDColorBlack, KDColorWhite),
  m_backgroundColor(KDColorWhite)
{
}

void MessageTableCell::setHighlighted(bool highlight) {
  HighlightCell::setHighlighted(highlight);
  KDColor backgroundColor = highlight? Palette::Select : m_backgroundColor;
  m_messageTextView.setBackgroundColor(backgroundColor);
}

void MessageTableCell::setMessage(I18n::Message text) {
  m_messageTextView.setMessage(text);
}

void MessageTableCell::setTextColor(KDColor color) {
  m_messageTextView.setTextColor(color);
}

void MessageTableCell::setMessageFont(KDFont::Size font) {
  m_messageTextView.setFont(font);
}

void MessageTableCell::setBackgroundColor(KDColor color) {
  m_backgroundColor = color;
  m_messageTextView.setBackgroundColor(color);
}

}
