#include <iostream>
#include <string>
#include <vector>
#include <openssl/evp.h>

struct Usuario {
    std::string nome;
    std::string senha;
};

struct Bloco {
    std::string dados;
    std::string hash;
    Bloco* anterior;
};

class Blockchain {
public:
    std::vector<Bloco*> blocos;

    void adicionarBloco(std::string dados) {
        Bloco* novoBloco = new Bloco();
        novoBloco->dados = dados;
        novoBloco->hash = calcularHash(dados);
        novoBloco->anterior = nullptr;
        blocos.push_back(novoBloco);
    }

    std::string calcularHash(std::string dados) {
        unsigned char hash[EVP_MD_size(EVP_sha256())];
        EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
        EVP_DigestUpdate(mdctx, dados.c_str(), dados.size());
        EVP_DigestFinal(mdctx, hash, NULL);
        EVP_MD_CTX_free(mdctx);

        std::string hashString;
        for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {
            char hex[3];
            sprintf(hex, "%02x", hash[i]);
            hashString += hex;
        }

        return hashString;
    }
};

class GerenciadorDeUsuarios {
public:
    std::vector<Usuario> usuarios;
    Blockchain blockchain;

    void adicionarUsuario(std::string nome, std::string senha) {
        Usuario novoUsuario;
        novoUsuario.nome = nome;
        novoUsuario.senha = senha;
        usuarios.push_back(novoUsuario);
        blockchain.adicionarBloco("Usuário adicionado: " + nome);
    }

    void removerUsuario(std::string nome) {
        for (auto it = usuarios.begin(); it != usuarios.end(); ++it) {
            if (it->nome == nome) {
                usuarios.erase(it);
                blockchain.adicionarBloco("Usuário removido: " + nome);
                return;
            }
        }
    }

    void atualizarUsuario(std::string nome, std::string novaSenha) {
        for (auto& usuario : usuarios) {
            if (usuario.nome == nome) {
                usuario.senha = novaSenha;
                blockchain.adicionarBloco("Usuário atualizado: " + nome);
                return;
            }
        }
    }

    void consultarUsuario(std::string nome) {
        for (const auto& usuario : usuarios) {
            if (usuario.nome == nome) {
                std::cout << "Usuário encontrado: " << usuario.nome << std::endl;
                return;
            }
        }
        std::cout << "Usuário não encontrado!" << std::endl;
    }
};

int main() {
    GerenciadorDeUsuarios gerenciadorDeUsuarios;

    while (true) {
        std::cout << "Opções:" << std::endl;
        std::cout << "1. Adicionar usuário" << std::endl;
        std::cout << "2. Remover usuário" << std::endl;
        std::cout << "3. Atualizar usuário" << std::endl;
        std::cout << "4. Consultar usuário" << std::endl;
        std::cout << "5. Sair" << std::endl;

        int opcao;
        std::cin >> opcao;

        switch (opcao) {
            case 1: {
                std::string nome, senha;
                std::cout << "Digite o nome do usuário: ";
                std::cin >> nome;
                std::cout << "Digite a senha do usuário: ";
                std::cin >> senha;
                gerenciadorDeUsuarios.adicionarUsuario(nome, senha);
                break;
            }
            case 2: {
                std::string nome;
                std::cout << "Digite o nome do usuário a ser removido: ";
                std::cin >> nome;
                gerenciadorDeUsuarios.removerUsuario(nome);
                break;
            }
            case 3: {
                std::string nome, novaSenha;
                std::cout << "Digite o nome do usuário a ser atualizado: ";
                std::cin >> nome;
                std::cout << "Digite a nova senha do usuário: ";
                std::cin >> novaSenha;
                gerenciadorDeUsuarios.atualizarUsuario(nome, novaSenha);
                break;
            }
            case 4: {
                std::string nome;
                std::cout << "Digite o nome do usuário a ser consultado: ";
                std::cin >> nome;
                gerenciadorDeUsuarios.consultarUsuario(nome);
                break;
            }
            case 5:
                return 0;
            default:
                std::cout << "Opção inválida!" << std::endl;
        }
    }

    return 0;
}