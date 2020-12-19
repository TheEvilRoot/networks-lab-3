#include <memory>
#include <iostream>

#include <QtCore>
#include <QApplication>
#include <QMainWindow>

#include "ui_main.h"

struct CRC {

	std::string source;
	std::vector<std::pair<std::string, std::string>> chunks;

	static std::string pad(const std::string &src, char c, int count) {
			if (src.length() < count) {
					return std::string(count - src.length(), c) + src;
			}
			return src;
	}

	static std::string sXor(const std::string &op1, const std::string &op2) {
			std::string buffer = op1.substr(1);
			for (auto i = 0; i < buffer.size(); i++) {
					if (i >= op2.size()) {
							qDebug() << "sXor out of op2(" << op2.size() << ") " << i << "\n";
					}
					buffer[i] = (buffer[i] == op2[i]) ? '0' : '1';
			}
			return buffer;
	}

	static std::string fromBinary(const std::string &source) {
		auto binary = source.substr();
		if (source.size() % 8 != 0)
			binary = std::string(source.size() % 8, '0') + source;

		std::string res;
		for (size_t i = 0; i < binary.size(); i += 8) {
			std::string v;
			if (i + 8 >= binary.size()) {
				v = binary.substr(i);
			} else {
				v = binary.substr(i, 8);
			}

			// 01100100
			uint8_t value = 0;
			for (auto j = 7; j >= 0; j--) {
				value |= ((uint8_t)(v[j] - '0') << (7 - j));
			}
			res += std::string(1, value);
		}
		return res;
	}

	static CRC doCrc(const std::string &source, size_t chunkSize, const std::string &polynom) {
			std::string string = source;
			if (string.size() % chunkSize != 0)
				string += std::string(string.size() % chunkSize, '0');
			std::string binary;
			for (auto cell : string)
					binary += pad(QString::number(cell, 2).toStdString(), '0', 8);
			std::cerr << "binary : " << binary << "\n";
			QList<std::string> chunks;
			for (size_t i = 0; i < binary.size(); i += chunkSize * 8) {
					if (i + (chunkSize * 8) >= binary.size()) {
							chunks.append(binary.substr(i));
					} else {
							chunks.append(binary.substr(i, chunkSize * 8));
					}
			}
			std::cerr << "chunks :\n";
			for (auto i = 0; i < chunks.size(); i++)
				std::cerr << i << " -> " << chunks[i] << '\n';

			std::vector<std::pair<std::string, std::string>> res;
			for (const auto & chunk : chunks) {
					auto picked = polynom.size();
					auto buffer = chunk.substr(0, picked);

					const auto iter = [](const auto &buffer, const auto &polynom, auto picked) {
						auto op = buffer[0] == '1' ? polynom : std::string(picked, '0');
						return sXor(buffer, op);
					};

					for (; picked < chunk.size(); picked++) {
						buffer = iter(buffer, polynom, picked) + std::string(1, chunk[picked]);
					}
					res.emplace_back(fromBinary(chunk), iter(buffer, polynom, picked));
			}

			return CRC{source, res};
	}
};

class MainWindow : public QMainWindow {
	std::unique_ptr<Ui::MainWindow> ui;

public:
	explicit MainWindow(QWidget *parent = nullptr): QMainWindow(parent) {
		ui = std::make_unique<Ui::MainWindow>();
		ui->setupUi(this);
		connect(ui->send, &QPushButton::clicked, [&]() { onSendClicked(); });

		setResult(nullptr);
	}

	void setResult(CRC *crc) {
		if (crc == nullptr) {
			ui->input->clear();
			ui->input->repaint();
			ui->out->clear();
			ui->out->repaint();
			ui->status->clear();
			ui->status->repaint();
		} else {
					ui->out->setText(QString::fromStdString(crc->source));
					ui->out->repaint();

					QString value;
					int index = 1; 
					for (const auto & [chunk, crc] : crc->chunks) {
						value += "Part " + QString::number(index++) +  "<br/>";
						value += escapeHtml(chunk);
						value += QString::fromStdString("<font color=red>" + crc + "</font><br>");
					}

					ui->status->setHtml(value);
					ui->status->repaint();
		}
	}

	void onSendClicked() {
		auto text = ui->input->text();
		if (!text.isEmpty()) {
					auto crc = CRC::doCrc(text.toStdString(), 2, "101111");
					setResult(&crc);
		}
	}

	static QString escapeHtml(std::string str) {
		return QString::fromStdString(str).replace(">", "&gt;").replace("<", "&lt;");
	}
};

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return QApplication::exec();
}
