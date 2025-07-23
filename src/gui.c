#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "verificador.h"  // Cabe�alho com fun��es e vari�veis usadas na verifica��o

// IDs dos bot�es e do campo de sa�da usados na interface gr�fica
#define ID_BTN_CARREGAR 101
#define ID_BTN_BLOOM_HASH 102
#define ID_BTN_HASH 103
#define ID_BTN_LINEAR 104
#define ID_BTN_RELATORIO 105
#define ID_EDIT_OUTPUT 201

HWND hwndOutput;  // Handle para a caixa de texto onde o output ser� exibido

// Fun��o para adicionar texto ao final da caixa de texto (sem apagar o conte�do anterior)
void print_to_output(const char* text) {
    int length = GetWindowTextLengthA(hwndOutput);
    SendMessageA(hwndOutput, EM_SETSEL, (WPARAM)length, (LPARAM)length);  // Move cursor para o fim
    SendMessageA(hwndOutput, EM_REPLACESEL, FALSE, (LPARAM)text);         // Insere o texto
    SendMessageA(hwndOutput, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");       // Insere quebra de linha
}

// Fun��o para substituir completamente o texto da caixa de sa�da
void set_output_text(const char* text) {
    SetWindowTextA(hwndOutput, text);
}

// Verifica��o usando filtro Bloom combinado com tabela hash (mais eficiente)
void verificar_bloom_hash_gui() {
    set_output_text("Iniciando verifica��o Bloom + Hash...\r\n");
    UpdateWindow(hwndOutput);  // Atualiza a janela para mostrar texto imediatamente

    memset(bloom, 0, sizeof(bloom));  // Zera o filtro Bloom
    free_hash_table();                 // Limpa tabela hash para nova verifica��o

    int duplicatas = 0;

    clock_t start = clock();

    // Percorre todos os emails
    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];
        char buf[256];

        if (bloom_possibly_contains(email)) { // Se filtro Bloom indica poss�vel presen�a
            if (search_hash(email)) {          // Verifica hash para confirma��o
                duplicatas++;
                snprintf(buf, sizeof(buf), "%s => J� cadastrado", email);
                print_to_output(buf);
            }
            else {
                insert_hash(email);             // Insere na hash
                snprintf(buf, sizeof(buf), "%s => Novo", email);
                print_to_output(buf);
            }
        }
        else {
            bloom_add(email);                 // Adiciona ao filtro Bloom
            insert_hash(email);               // Insere na hash
            snprintf(buf, sizeof(buf), "%s => Novo", email);
            print_to_output(buf);
        }
    }

    clock_t end = clock();
    tempo_bloom_hash = (double)(end - start) / CLOCKS_PER_SEC;

    // Exibe resumo em MessageBox
    char summary[512];
    snprintf(summary, sizeof(summary),
        "=== RESULTADO BLOOM + HASH ===\n"
        "Tempo total: %.4f segundos\n"
        "Total de emails processados: %d\n"
        "Total de duplicatas encontradas: %d\n",
        tempo_bloom_hash, total_emails, duplicatas);

    MessageBoxA(NULL, summary, "Resumo Bloom + Hash", MB_OK | MB_ICONINFORMATION);

    free_hash_table();  // Libera tabela hash ao final
}

// Verifica��o usando somente tabela hash (busca r�pida)
void verificar_hash_gui() {
    set_output_text("Iniciando verifica��o Hash padr�o...\r\n");
    UpdateWindow(hwndOutput);

    free_hash_table();  // Limpa hash

    int duplicatas = 0;

    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];
        char buf[256];

        if (search_hash(email)) {
            duplicatas++;
            snprintf(buf, sizeof(buf), "%s => J� cadastrado", email);
            print_to_output(buf);
        }
        else {
            insert_hash(email);
            snprintf(buf, sizeof(buf), "%s => Novo", email);
            print_to_output(buf);
        }
    }

    clock_t end = clock();
    tempo_hash = (double)(end - start) / CLOCKS_PER_SEC;

    // Mostra resumo do processo
    char summary[512];
    snprintf(summary, sizeof(summary),
        "=== RESULTADO HASH PADR�O ===\n"
        "Tempo total: %.4f segundos\n"
        "Total de emails processados: %d\n"
        "Total de duplicatas encontradas: %d\n",
        tempo_hash, total_emails, duplicatas);

    MessageBoxA(NULL, summary, "Resumo Hash Padr�o", MB_OK | MB_ICONINFORMATION);

    free_hash_table();
}

// Verifica��o usando busca linear
void verificar_linear_gui() {
    set_output_text("Iniciando verifica��o Busca Linear...\r\n");
    UpdateWindow(hwndOutput);

    // Aloca array para armazenar emails j� vistos durante a verifica��o linear
    char (*emails_linear)[MAX_EMAIL_LEN] = malloc(sizeof(char[MAX_EMAIL_LEN]) * MAX_EMAILS);
    if (!emails_linear) {
        MessageBoxA(NULL, "Erro de mem�ria ao alocar emails_linear.", "Erro", MB_OK | MB_ICONERROR);
        return;
    }

    int count = 0;     // Contador de emails �nicos armazenados
    int duplicatas = 0;

    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];
        char buf[256];

        int found = 0;
        // Busca linear para verificar se email j� foi armazenado
        for (int j = 0; j < count; j++) {
            if (strcmp(emails_linear[j], email) == 0) {
                found = 1;
                break;
            }
        }

        if (found) {
            duplicatas++;
            snprintf(buf, sizeof(buf), "%s => J� cadastrado", email);
            print_to_output(buf);
        }
        else {
            strcpy(emails_linear[count++], email);  // Adiciona novo email
            snprintf(buf, sizeof(buf), "%s => Novo", email);
            print_to_output(buf);
        }
    }

    clock_t end = clock();
    tempo_linear = (double)(end - start) / CLOCKS_PER_SEC;

    // Exibe resumo do tempo e resultados
    char summary[512];
    snprintf(summary, sizeof(summary),
        "=== RESULTADO BUSCA LINEAR ===\n"
        "Tempo total: %.4f segundos\n"
        "Total de emails processados: %d\n"
        "Total de duplicatas encontradas: %d\n",
        tempo_linear, total_emails, duplicatas);

    MessageBoxA(NULL, summary, "Resumo Busca Linear", MB_OK | MB_ICONINFORMATION);

    free(emails_linear);  // Libera mem�ria alocada
}

// Mostra relat�rio comparativo dos tempos de execu��o dos m�todos
void mostrar_relatorio_gui() {
    set_output_text("");  // Limpa o conte�do da caixa de sa�da antes de imprimir

    char buffer[256];

    print_to_output("===== RELAT�RIO DE TEMPOS E RESULTADOS =====");

    // Exibe tempos ou mensagem caso o m�todo n�o tenha sido executado
    if (tempo_bloom_hash >= 0) {
        snprintf(buffer, sizeof(buffer), "Filtro Bloom + Hash: %.4f segundos", tempo_bloom_hash);
        print_to_output(buffer);
    } else {
        print_to_output("Filtro Bloom + Hash: Ainda n�o executado");
    }

    if (tempo_hash >= 0) {
        snprintf(buffer, sizeof(buffer), "Hash padr�o: %.4f segundos", tempo_hash);
        print_to_output(buffer);
    } else {
        print_to_output("Hash padr�o: Ainda n�o executado");
    }

    if (tempo_linear >= 0) {
        snprintf(buffer, sizeof(buffer), "Busca linear: %.4f segundos", tempo_linear);
        print_to_output(buffer);
    } else {
        print_to_output("Busca linear: Ainda n�o executado");
    }

    print_to_output(""); // Linha em branco para espa�amento

    // Compara��es percentuais entre os m�todos
    if (tempo_bloom_hash >= 0 && tempo_hash >= 0) {
        double diff = tempo_hash - tempo_bloom_hash;
        double perc = (diff / tempo_hash) * 100.0;
        snprintf(buffer, sizeof(buffer),
            "Bloom + Hash � %.2f%% %s que Hash padr�o",
            perc < 0 ? -perc : perc,
            perc < 0 ? "mais lento" : "mais r�pido");
        print_to_output(buffer);
    }

    if (tempo_hash >= 0 && tempo_linear >= 0) {
        double diff = tempo_linear - tempo_hash;
        double perc = (diff / tempo_linear) * 100.0;
        snprintf(buffer, sizeof(buffer),
            "Hash padr�o � %.2f%% %s que Busca linear",
            perc < 0 ? -perc : perc,
            perc < 0 ? "mais lento" : "mais r�pido");
        print_to_output(buffer);
    }

    print_to_output("");

    snprintf(buffer, sizeof(buffer), "Total de emails processados: %d", total_emails);
    print_to_output(buffer);

    // Observa��es gerais para o usu�rio
    print_to_output("Observa��es:");
    if (tempo_bloom_hash < 0 && tempo_hash < 0 && tempo_linear < 0) {
        print_to_output("- Nenhum m�todo foi executado ainda.");
    } else {
        if (tempo_bloom_hash >= 0 && tempo_hash >= 0 && tempo_linear >= 0) {
            print_to_output("- Bloom + Hash tende a ser mais eficiente para listas muito grandes.");
            print_to_output("- Busca linear � mais lenta e deve ser evitada em grandes volumes.");
            print_to_output("- Hash padr�o oferece equil�brio entre simplicidade e desempenho.");
        } else {
            print_to_output("- Execute os m�todos para obter an�lises comparativas.");
        }
    }
}

// Fun��o que trata mensagens da janela (eventos e a��es do usu�rio)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_CREATE: {  // Quando janela � criada, criar bot�es e campo de texto
            CreateWindowA("BUTTON", "Carregar lista CSV", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                10, 10, 150, 30, hwnd, (HMENU)ID_BTN_CARREGAR, NULL, NULL);

            CreateWindowA("BUTTON", "Bloom + Hash", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                170, 10, 150, 30, hwnd, (HMENU)ID_BTN_BLOOM_HASH, NULL, NULL);

            CreateWindowA("BUTTON", "Hash padr�o", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                330, 10, 150, 30, hwnd, (HMENU)ID_BTN_HASH, NULL, NULL);

            CreateWindowA("BUTTON", "Busca Linear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                490, 10, 150, 30, hwnd, (HMENU)ID_BTN_LINEAR, NULL, NULL);

            CreateWindowA("BUTTON", "Mostrar relat�rio", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                650, 10, 150, 30, hwnd, (HMENU)ID_BTN_RELATORIO, NULL, NULL);

            // Caixa de texto multilinha para exibir os resultados
            hwndOutput = CreateWindowA("EDIT", "", 
                WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                10, 50, 790, 400, hwnd, (HMENU)ID_EDIT_OUTPUT, NULL, NULL);
            break;
        }
        case WM_COMMAND: {  // Trata eventos de clique dos bot�es
            switch(LOWORD(wParam)) {
                case ID_BTN_CARREGAR: {
                    // Abre di�logo para carregar arquivo CSV
                    OPENFILENAME ofn;
                    char filename[260] = "";

                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = sizeof(filename);
                    ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    if (GetOpenFileNameA(&ofn)) {  // Se usu�rio selecionou arquivo
                        if (carregar_emails(filename)) {  // Carrega emails do CSV
                            char msg[100];
                            snprintf(msg, sizeof(msg), "======== LISTA CARREGADA ========\r\nTotal de emails: %d\r\n", total_emails);
                            set_output_text(msg);
                        }
                        else {
                            set_output_text("Falha ao carregar a lista.\r\n");
                        }
                    }
                    break;
                }
                case ID_BTN_BLOOM_HASH:
                    if (total_emails == 0) {
                        MessageBoxA(hwnd, "Por favor, carregue uma lista primeiro.", "Erro", MB_OK | MB_ICONERROR);
                    } else {
                        verificar_bloom_hash_gui();
                    }
                    break;
                case ID_BTN_HASH:
                    if (total_emails == 0) {
                        MessageBoxA(hwnd, "Por favor, carregue uma lista primeiro.", "Erro", MB_OK | MB_ICONERROR);
                    } else {
                        verificar_hash_gui();
                    }
                    break;
                case ID_BTN_LINEAR:
                    if (total_emails == 0) {
                        MessageBoxA(hwnd, "Por favor, carregue uma lista primeiro.", "Erro", MB_OK | MB_ICONERROR);
                    } else {
                        verificar_linear_gui();
                    }
                    break;
                case ID_BTN_RELATORIO:
                    mostrar_relatorio_gui();
                    break;
            }
            break;
        }
        case WM_DESTROY: {  // Fecha a aplica��o quando a janela for fechada
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);  // Processo padr�o para outras mensagens
    }
    return 0;
}

// Ponto de entrada da aplica��o Windows GUI
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "VerificadorWindowClass";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;  // Define a fun��o de callback para janela
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);  // Registra a classe da janela

    // Cria a janela principal
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Verificador de Duplicatas - Interface Gr�fica",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 830, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);  // Exibe a janela

    MSG msg = {0};
    // Loop principal de mensagens do Windows
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
