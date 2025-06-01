#include <stdlib.h>
#include "../../include/entities/command.h"

void free_pipeline(t_pipeline *pipeline)
{
    while (pipeline) {
        t_pipeline *next = pipeline->next;
        free_cmd(pipeline->cmds);
        free(pipeline);
        pipeline = next;
    }
}

void free_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;
    
    // argv の解放
    if (cmd->argv) {
        for (int i = 0; cmd->argv[i]; i++) {
            free(cmd->argv[i]);
        }
        free(cmd->argv);
    }
    
    // リダイレクトの解放
    free_redirect(cmd->redirects);
    
    // 次のコマンドの解放（パイプチェーン）
    free_cmd(cmd->next);
    
    free(cmd);
}

void free_redirect(t_cmd_redirect *redirect)
{
    while (redirect) {
        t_cmd_redirect *next = redirect->next;
        free(redirect->file);
        free(redirect);
        redirect = next;
    }
}