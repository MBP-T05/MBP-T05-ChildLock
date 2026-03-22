import fitz
import sys

def extract_pdf(pdf_path, out_path):
    try:
        doc = fitz.open(pdf_path)
        with open(out_path, 'w', encoding='utf-8') as f:
            for page_num in range(len(doc)):
                page = doc.load_page(page_num)
                text = page.get_text()
                f.write(f"--- Page {page_num + 1} ---\n")
                f.write(text + "\n\n")
        print("PDF extracted successfully.")
    except Exception as e:
        print(f"Error extracting PDF: {e}")

if __name__ == "__main__":
    pdf_path = r"C:\Users\fish0\OneDrive\바탕 화면\모비우스 부트캠프\1. PBL iso26262 개발 프로젝트\공유 파일 및 기타 파일\01 모비우스 부트캠프 PBL_(1) ISO26262_OT_구직자.pdf"
    out_path = r"C:\Users\fish0\.gemini\antigravity\brain\d696c5d4-907c-4e6e-bc04-2a070b2afe2f\pdf_content.txt"
    extract_pdf(pdf_path, out_path)
